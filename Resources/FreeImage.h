// FreeImage resource.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _FREE_IMAGE_H_
#define _FREE_IMAGE_H_

#include <Resources/Texture2D.h>
#include <Resources/IResourcePlugin.h>
#include <Logging/Logger.h>

#include <string>
#include <iostream>
#include <fstream>

#include <Resources/Exceptions.h>
#include <Resources/File.h>
#include <Utils/Convert.h>

//#include <cstring> // includes memcpy

#include <Meta/FreeImage.h>

namespace OpenEngine {
namespace Resources {

using namespace std;


// without the template argument (why should the user specify the type
// of the resource, which can in general not be decided until the
// resource is loaded???!! - a paradox?)
class FImage: public ITexture2D {
private:
    bool loaded;
    string filename;            //!< file name
public:
    FImage() : loaded(false) {} 

    /**
     * Constructor
     *
     * @param file name of file to load.
     */
    FImage(string file)
        : ITexture2D(), loaded(false), filename(file) {}

    virtual ~FImage() {}

    // resource methods
    virtual void Load() {
        if (loaded) return;

        FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename.c_str(), 0);
        FIBITMAP* img = FreeImage_Load(format, filename.c_str());
        if (!img) throw ResourceException("Error loading file: " + filename + ".");
        
        // color types:
        // 'uint8   : one channel 8 bit int
        // 'uint16  : one channel 16 bit int
        // 'float32 : one channel 32 bit float
        // 'rgb     : 24 bit rgb color
        // 'rgba    : 32 bit rgba color
        // 'rgb32f  : 3 x 32 bit float color  
        // 'rgba32f : 4 x 32 bit float color  

    
        this->width  = FreeImage_GetWidth(img);
        this->height = FreeImage_GetHeight(img);
        unsigned int pixels = width * height;
        BYTE *bits = FreeImage_GetBits(img);
        unsigned int bpp = FreeImage_GetBPP(img);
        FREE_IMAGE_TYPE image_type = FreeImage_GetImageType(img);
        bool loaded = true;

        unsigned int i;
        switch(image_type) {
        case FIT_BITMAP: 
            {
                switch(bpp) {
                case 8:
                    { 
                        this->channels = 1;
                        this->format = LUMINANCE; 
                        this->type = Types::UBYTE;
                        char* data = new char[pixels];
                        this->data = data;
                        memcpy(data, bits, sizeof(char) * pixels); 
                        break;
                    }
                case 24: 
                    {
                        this->channels = 3;
                        this->format = RGB; 
                        this->type = Types::UBYTE;
                        char* data = new char[pixels*3];
                        this->data = data;
                        RGBTRIPLE* pix = (RGBTRIPLE*) bits;
                        for (i = 0; i < pixels; ++i) {
                            data[i*3]   = pix[i].rgbtRed;
                            data[i*3+1] = pix[i].rgbtGreen;
                            data[i*3+2] = pix[i].rgbtBlue;
                        }
                        // set_bitmap_scm(width, height, "rgb", data);
                        break;
                    }
                case 32:
                    {
                        this->channels = 4;
                        this->format = RGBA; 
                        this->type = Types::UBYTE;
                        char* data = new char[pixels*4];
                        this->data = data;
                        RGBQUAD* pix = (RGBQUAD*) bits;
                        for (i = 0; i < pixels; ++i) {
                            data[i*4]   = pix[i].rgbRed;
                            data[i*4+1] = pix[i].rgbGreen;
                            data[i*4+2] = pix[i].rgbBlue;
                            data[i*4+3] = pix[i].rgbReserved;
                        }
                        // set_bitmap_scm(width, height, "rgba", data);
                        break;
                    }
                default:
                    loaded = false;
                }
                break;   
            }
        case FIT_UINT16:
            {
                this->channels = 1;
                this->format = LUMINANCE; 
                this->type = Types::USHORT;
                unsigned short* data = new (unsigned short)(pixels);
                this->data = data;
                memcpy(data, bits, sizeof(unsigned short) * pixels); 
                // set_bitmap_scm(width, height, "uint16", (char*)data); 
                break;
            }
        case FIT_FLOAT:
            {
                this->channels = 1;
                this->format = LUMINANCE; 
                this->type = Types::FLOAT;
                float* data = new float[pixels];
                this->data = data;
                memcpy(data, bits, sizeof(float) * pixels); 
                // set_bitmap_scm(width, height, "float32", (char*)data); 
                break;
            }
        default:
            loaded = false;
        }

        FreeImage_Unload(img);

    }

    virtual void Unload() {
        switch (this->type) {
        case Types::UBYTE:
            delete[] (char*)this->data;
            break;
        case Types::USHORT:
            delete[] (unsigned short*)this->data;
            break; 
        case Types::FLOAT:
            delete[] (float*)this->data;
            break;
        default:
            logger.warning << "Freeimage texture: Cannot unload unsupported texture type." << logger.end;
            break;
        }
        this->loaded = false;
    }

    virtual ITexture2D* Clone() {
        throw ResourceException("Clone() not implemented.");
    }

    virtual unsigned int GetChannelSize() {
        throw ResourceException("GetChannelSize() not implemented since it is redundant (see colorformat and type!).");
    }
    
    virtual void Reverse() {
        throw ResourceException("Reverse() not implemented since it is a silly operation to have on a texture resource.");
    }

    virtual void ReverseVertecally() {
        throw ResourceException("ReverseVertically() not implemented since it is a silly operation to have on a texture resource.");
    }
    
    virtual void ReverseHorizontally() {
        throw ResourceException("ReverseHorizontally() not implemented since it is a silly operation to have on a texture resource.");
    }
};



/**
 * FreeImage resource.
 *
 * @class FreeImage FreeImage.h Resources/FreeImage.h
 */
template <class T>
class FreeImage : public Texture2D<T> {
private:
    bool loaded;
    string filename;            //!< file name

public:
    FreeImage() : loaded(false) {} // needed by serialization

    /**
     * Constructor
     *
     * @param file name of file to load.
     */
    FreeImage(string file)
        : Texture2D<T>(), loaded(false), filename(file) {}

    ~FreeImage() {}

    // resource methods
    virtual void Load() {
    if (loaded) return;

    FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(filename.c_str(),0);
	FIBITMAP* imagen = FreeImage_Load(formato, filename.c_str());
    if (!imagen)
        throw ResourceException("Error loading file: " + filename +
                                ". Description: " + "File not found");
                                //@todo: append freeimage getError()

    this->width = FreeImage_GetWidth(imagen);
    this->height = FreeImage_GetHeight(imagen);
    this->channels = FreeImage_GetBPP(imagen)/(sizeof(T)*8);

    /*
    unsigned int pitch =FreeImage_GetPitch(imagen);
    logger.info << "pitch: " << pitch << logger.end;

    FREE_IMAGE_TYPE type = FreeImage_GetImageType(imagen);
    logger.info << "type: " << TypeToString(type) << logger.end;

    FREE_IMAGE_COLOR_TYPE colorType = FreeImage_GetColorType(imagen);
    logger.info << "colorType: " << ColorTypeToString(colorType) << logger.end;
    */
    unsigned int lineWidth = this->width * this->channels;
    unsigned long size = lineWidth * this->height;
    T* d = new T[size];
    this->data = d;

    // @todo: do raw memcopy instead of for loops
	T* pixels = (T*)FreeImage_GetBits(imagen);
    std::memcpy(d, pixels, size*sizeof(T));

    /*
        logger.info << "FI_RGBA_RED: " << FI_RGBA_RED << logger.end; 
        logger.info << "FI_RGBA_GREEN: " << FI_RGBA_GREEN << logger.end; 
        logger.info << "FI_RGBA_BLUE: " << FI_RGBA_BLUE << logger.end; 
        logger.info << "FI_RGBA_ALPHA: " << FI_RGBA_ALPHA << logger.end; 
*/

    /*
    unsigned int c = this->channels;
    unsigned int w = this->width;
    unsigned int h = this->height;

    unsigned int bytespp = FreeImage_GetLine(imagen) / w;
    logger.info << "bytespp: " << bytespp << logger.end;
    for (unsigned int y = 0; y < h; y++) {
        T* bits = (T*)FreeImage_GetScanLine(imagen, y);

        for (unsigned int x = 0; x < w; x++) {

            unsigned int id = (x+y*w)*c;
            //#define USE_LOOP
#ifdef USE_LOOP
            for (unsigned int ch = 0; ch < c; ch++) {
                d[id + ch] = bits[accountForEndian(ch)];
            }
#else
            d[id + 0] = bits[FI_RGBA_RED];
            if (c >= 2)
                d[id + 1] = bits[FI_RGBA_GREEN];
            if (c >= 3)
                d[id + 2] = bits[FI_RGBA_BLUE];
            if (c >= 4)
                d[id + 3] = bits[FI_RGBA_ALPHA];
#endif
            // jump to next pixel
            bits += bytespp;
        }
    }
    */
	FreeImage_Unload(imagen);
    
    //flip vertecally
    //ReverseVertecally();

    if (typeid(T) == typeid(float))
        this->format = RGBA32F; //@todo: convert pixels from BGRA to RGBA
    else
        this->format = ITexture::ColorFormatFromChannelsBGR(this->channels);

    /*
    logger.info << "loaded: " << filename 
                << " (" << colorFomatToString(this->format) << "), "
                << this->width << "x" << this->height << ":"
                << (int)this->channels << logger.end;
    */
    loaded = true;
}


    virtual void Unload() {}
};

/**
 * FreeImage texture resource plug-in.
 *
 * @class FreeImagePlugin FreeImage.h Resources/FreeImage.h
 */
class FreeImagePlugin : public IResourcePlugin<ITextureResource> {
public:
    FreeImagePlugin();
    ITextureResourcePtr CreateResource(string file);
};

/**
 * FreeImage texture resource plug-in.
 *
 * @class UCharFreeImagePlugin FreeImage.h Resources/FreeImage.h
 */
class UCharFreeImagePlugin : public IResourcePlugin<UCharTexture2D> {
public:
    UCharFreeImagePlugin();
    UCharTexture2DPtr CreateResource(string file);
};

class FloatFreeImagePlugin : public IResourcePlugin<FloatTexture2D> {
public:
    FloatFreeImagePlugin();
    FloatTexture2DPtr CreateResource(string file);
};

} //NS Resources
} //NS OpenEngine

#endif // _FREE_IMAGE_H_
