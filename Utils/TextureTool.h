#ifndef _TEXTURE_TOOL_
#define _TEXTURE_TOOL_

#include <Resources/Texture2D.h>

#include <Resources/Directory.h>

#include <string>
#include <limits>
#include <Meta/FreeImage.h>

using namespace OpenEngine::Resources;

// on linux the following constants are not defined in FreeImage.h for
// some reason. Got these values from:
// http://freeimage.sourceforge.net/fip/FreeImage_8h-source.html
#ifndef PNG_Z_BEST_SPEED
#define PNG_Z_BEST_SPEED 0x0001  // save using ZLib level 1 compression flag (default value is 6)
#endif
#ifndef PNG_Z_BEST_COMPRESSION
#define PNG_Z_BEST_COMPRESSION 0x0009  // save using ZLib level 9 compression flag (default value is 6)
#endif

/** FreeImage error handler
@param fif Format / Plugin responsible for the error
@param message Error message
*/
void FreeImageErrorHandler(FREE_IMAGE_FORMAT fif, const char *message) {
    printf("\n*** ");
    if (fif != FIF_UNKNOWN) {
        printf("%s Format\n", FreeImage_GetFormatFromFIF(fif));
    }
    printf("%s\n", message);
    printf(" ***\n");
}

template <class T>
class TextureTool {
 private:
    static vector<Texture2DPtr(T)> ToLayers(Texture3DPtr(T) tex) {
    const unsigned int w = tex->GetWidth();
    const unsigned int h = tex->GetHeight();
    const unsigned int d = tex->GetDepth();
    const unsigned int c = tex->GetChannels();
    const T* din = tex->GetData();

    vector<Texture2DPtr(T)> list;
    for (unsigned int z=0; z<d; z++) {
        typename Texture2DPtr(T) output(new Texture2D<T>(w,h,c));
        T* dout = output->GetData();
        for (unsigned int y=0; y<h; y++) {
            for (unsigned int x=0; x<w; x++) {
                for (unsigned int ch=0; ch<c; ch++) {
                    dout[(x+y*w)*c+ch] = din[(x+y*w+z*w*h)*c+ch]; 
                }
            }
        }
        list.push_back(output);
    }
    return list;
}

 public:
 static void DumpTexture(Texture2DPtr(T) tex, std::string filename) {
        FreeImage_Initialise();
        FreeImage_SetOutputMessage(FreeImageErrorHandler);

        const unsigned int w = tex->GetWidth();
        const unsigned int h = tex->GetHeight();
        const unsigned int c = tex->GetChannels(); 
        const unsigned int cs = tex->GetChannelSize(); 
        const unsigned int bpp = 8*cs*c; 
        /*
        logger.info << "saving: " << w << "x" << h << ":" << c 
                    << "-" << bpp << logger.end;
        */
        FIBITMAP* bitmap = NULL;
        if (typeid(T) ==typeid(float)) {
            bitmap = FreeImage_AllocateT(FIT_RGBAF, w, h, bpp);
        } else if (typeid(T) ==typeid(unsigned char)) {
            bitmap = FreeImage_Allocate(w, h, bpp);
        } else
            throw Exception("unsupported texture dump format on alloc.");

        if (!bitmap)
            throw Exception("Error: allocation failed");

        unsigned int bytespp = FreeImage_GetLine(bitmap) / w;
        T* data = tex->GetData();
        for (unsigned int y = 0; y < h; y++) { 
            T* bits = reinterpret_cast<T*>
                (FreeImage_GetScanLine(bitmap,y));
            for (unsigned int x = 0; x < w; x++) {

        if (typeid(T) ==typeid(float)) {
                /*
                float scale = 1.0f;
                bits[x*4+0] = 1.0 * scale;
                bits[x*4+1] = 1.0 * scale;
                bits[x*4+2] = 1.0 * scale;
                bits[x*4+3] = data[x+y*width] * scale;
                */
                for (unsigned int ch = 0; ch < c; ch++) {
                    bits[x*c+ch] = data[(x+y*w)*c+ch];
                }
        } else if (typeid(T) ==typeid(unsigned char)) {
                bits[FI_RGBA_BLUE]   = data[(x+y*w)*c + 0];
                if (c >= 2)
                bits[FI_RGBA_GREEN] = data[(x+y*w)*c + 1];
                if (c >= 3)
                bits[FI_RGBA_RED]  = data[(x+y*w)*c + 2];
                if (c >= 4)
                bits[FI_RGBA_ALPHA] = data[(x+y*w)*c + 3];                

                // jump to next pixel
                bits += bytespp;
        }
            }
        }

        if (typeid(T) ==typeid(float)) {
            if ( !FreeImage_Save(FIF_EXR, bitmap, filename.c_str(), EXR_FLOAT) )
                throw Exception("Failed to save image: " + filename);
        } else if (typeid(T) ==typeid(unsigned char)) {
            if ( !FreeImage_Save(FIF_PNG, bitmap, filename.c_str(),
                                 PNG_Z_BEST_COMPRESSION) )
                                 //PNG_Z_BEST_SPEED) )
                throw Exception("Failed to save image: " + filename);
        } else
            throw Exception("unsupported texture dump format.");

        FreeImage_Unload(bitmap);
        FreeImage_DeInitialise();
    }

    static void DumpTexture(Texture3DPtr(T) input, std::string foldername) {
        if (!Directory::Exists(foldername)) {
            Directory::Make(foldername);
        }

        vector<Texture2DPtr(T) > texList = ToLayers(input);

        int count = 1;
        typename vector<Texture2DPtr(T) >::iterator itr = texList.begin();
        while (itr != texList.end()) {
            Texture2DPtr(T) tex = *itr;
            std::string filename = foldername + "/";

            // @todo: make this general
            if (count<10) filename += "0";
            if (count<100) filename += "0";

            filename += Convert::ToString(count);

            std::string fileExtension = ".";
            if (typeid(T) ==typeid(float)) {
                fileExtension += "exr";
            } else if (typeid(T) ==typeid(unsigned char)) {
                fileExtension += "png";
            } else
                throw Exception("unsupported texture dump format.");

            TextureTool<T>::DumpTexture(tex, filename + fileExtension);

            count++;
            ++itr;
        }
    }
    /*
    static void DumpTexture(UCharTexture2DPtr tex, std::string filename) {
        FreeImage_Initialise();
        //cout << "FreeImage vesrion: " << FreeImage_GetVersion() << endl;
        //cout << FreeImage_GetCopyrightMessage() << endl << endl;

        const unsigned int w = tex->GetWidth();
        const unsigned int h = tex->GetHeight();
        const unsigned int c = tex->GetChannels(); 
        const unsigned int cs = tex->GetChannelSize(); 
        const unsigned int bpp = 8*cs*c; 

        unsigned char* data = tex->GetData();

        FIBITMAP* bitmap = FreeImage_Allocate(w, h, bpp);
        if (!bitmap)
            throw Exception("Error: allocation failed");

        unsigned int bytespp = FreeImage_GetLine(bitmap) / w;
        for (unsigned int y = 0; y < h; y++) {
            BYTE* bits = (BYTE*)FreeImage_GetScanLine(bitmap, y);
            for (unsigned int x = 0; x < w; x++) {

                bits[FI_RGBA_RED]   = data[(x+y*w)*c + 0];
                if (c >= 2)
                bits[FI_RGBA_GREEN] = data[(x+y*w)*c + 1];
                if (c >= 3)
                bits[FI_RGBA_BLUE]  = data[(x+y*w)*c + 2];
                if (c >= 4)
                bits[FI_RGBA_ALPHA] = data[(x+y*w)*c + 3];
                
                // jump to next pixel
                bits += bytespp;
            }
        }


//         // Draws a gradient from blue to green:
//         RGBQUAD color;
//         for (unsigned int i=0 ; i < width; i++) {
//             for (unsigned int j=0; j < height; j++) {
//                 unsigned char pixel = data[i+j*width];
//                 color.rgbRed = pixel;
//                 color.rgbGreen = pixel;
//                 color.rgbBlue = pixel;
//                 FreeImage_SetPixelColor(bitmap, i, height-1-j, &color);
//                 // Notice how we're calling the & operator on color
//                 // so that we can pass a pointer to the color struct.
//             }
//         }

//         int pitch = ((((depth * width) + 31) / 32) * 4);
//         bitmap = 
//             FreeImage_ConvertFromRawBits(data, width, height, pitch, depth,
//                                          FI_RGBA_RED_MASK, 
//                                          FI_RGBA_GREEN_MASK, 
//                                          FI_RGBA_BLUE_MASK);
//         if (!bitmap)
//             throw Exception("Error: allocation failed");


        //if ( !FreeImage_Save(FIF_PNG, bitmap, filename.c_str(), PNG_Z_BEST_COMPRESSION) )
        if ( !FreeImage_Save(FIF_PNG, bitmap, filename.c_str(), PNG_Z_BEST_SPEED) )
            throw Exception("Failed to save image: " + filename);
        FreeImage_Unload(bitmap);
        FreeImage_DeInitialise();
    }
*/
};

#endif // _TEXTURE_TOOL_
