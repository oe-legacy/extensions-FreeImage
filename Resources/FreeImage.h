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
    /*
	unsigned char* pixels = (unsigned char*)FreeImage_GetBits(imagen);
    std::memcpy(data, pixels, size);
    */

    unsigned int c = this->channels;
    unsigned int w = this->width;
    unsigned int h = this->height;
    /*
        logger.info << "FI_RGBA_RED: " << FI_RGBA_RED << logger.end; 
        logger.info << "FI_RGBA_GREEN: " << FI_RGBA_GREEN << logger.end; 
        logger.info << "FI_RGBA_BLUE: " << FI_RGBA_BLUE << logger.end; 
        logger.info << "FI_RGBA_ALPHA: " << FI_RGBA_ALPHA << logger.end; 
*/
    unsigned int bytespp = FreeImage_GetLine(imagen) / w;
    logger.info << "bytespp: " << bytespp << logger.end;
    for (unsigned int y = 0; y < h; y++) {
        T* bits = (T*)FreeImage_GetScanLine(imagen, y);

        for (unsigned int x = 0; x < w; x++) {

            unsigned int id = (x+y*w)*c;
#define USE_LOOP
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
	FreeImage_Unload(imagen);
    
    //flip vertecally
    //ReverseVertecally();

    if (typeid(T) == typeid(float))
        this->format = RGBA32F;
    else
        this->format = ITexture::ColorFormatFromChannelsBGR(this->channels);

    logger.info << "loaded: " << filename 
                << " (" << colorFomatToString(this->format) << "), "
                << this->width << "x" << this->height << ":"
                << (int)this->channels << logger.end;
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
