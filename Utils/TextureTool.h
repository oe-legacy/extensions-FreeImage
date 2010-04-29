#ifndef _TEXTURE_TOOL_
#define _TEXTURE_TOOL_

#include <Resources/Texture2D.h>
#include <string>

using namespace OpenEngine::Resources;

#include <FreeImage.h>
#include <limits>

class TextureTool {
 public:
    static void DumpTexture(FloatTexture2DPtr tex, std::string filename) {
        FreeImage_Initialise();

        const unsigned int width = tex->GetWidth();
        const unsigned int height = tex->GetHeight();
        const unsigned int bpp = 8*sizeof(FIRGBAF); 

        FIBITMAP* bitmap = FreeImage_AllocateT(FIT_RGBAF, width, height, bpp);
        if (!bitmap)
            throw Exception("Error: allocation failed");

        float* data = tex->GetData();
        for (unsigned int y = 0; y < height; y++) { 
            float* bits = reinterpret_cast<float*>
                (FreeImage_GetScanLine(bitmap,y));
            for (unsigned int x = 0; x < width; x++) {
                float scale = 1.0f;

                bits[x*4+0] = 1.0 * scale;//data[x+y*width] * scale;
                bits[x*4+1] = 1.0 * scale;//data[x+y*width] * scale;
                bits[x*4+2] = 1.0 * scale;//0.0;//data[x+y*width] * scale;
                bits[x*4+3] = data[x+y*width] * scale;

                /*
                bits[x*4+0] = data[x+y*width] *scale;
                bits[x*4+1] = data[x+y*width] *scale;
                bits[x*4+2] = data[x+y*width] *scale;
                bits[x*4+3] = scale;//data[x+y*width] *scale;
                */

                //bits[FI_RGBA_RED] = 1.0;
                //bits[FI_RGBA_GREEN] = 1.0;
                //bits[FI_RGBA_BLUE] = 100000.0;
                //bits[FI_RGBA_ALPHA] = data[x+y*width];
            }
        }

        if ( !FreeImage_Save(FIF_EXR, bitmap, filename.c_str(), EXR_FLOAT) )
          throw Exception("Failed to save image: " + filename);
        FreeImage_Unload(bitmap);
        FreeImage_DeInitialise();
    }

    static void DumpTexture(UCharTexture2DPtr tex, std::string filename) {
        FreeImage_Initialise();
        //cout << "FreeImage vesrion: " << FreeImage_GetVersion() << endl;
        //cout << FreeImage_GetCopyrightMessage() << endl << endl;

        const unsigned int width = tex->GetWidth();
        const unsigned int height = tex->GetHeight();
        //const unsigned int channels = tex->GetChannels(); 

        if(tex->GetType() != Types::UBYTE)
           throw Exception("not a byte texture");
        unsigned char* data = (unsigned char*)tex->GetVoidDataPtr();

        //for (unsigned int i = 0; i<height*width; i++)
        //  data[i] = 255;

        //logger.info << width << "x" << height << ":" << channels << logger.end;

        FIBITMAP* bitmap = NULL;

        // since we are outputting thre 8 bit RGB values
        const unsigned int bpp = 8*4;
        bitmap = FreeImage_Allocate(width, height, bpp);
        if (!bitmap)
            throw Exception("Error: allocation failed");
        //bitmap = FreeImage_ConvertTo32Bits(bitmap);

        // Calculate the number of bytes per pixel (3 for 24-bit or 4 for 32-bit) 

        int bytespp = FreeImage_GetLine(bitmap) / FreeImage_GetWidth(bitmap);        
        for(unsigned int y = 0; y < FreeImage_GetHeight(bitmap); y++) { 
            BYTE* bits = (BYTE*)FreeImage_GetScanLine(bitmap, y);
            for(unsigned int x = 0; x < FreeImage_GetWidth(bitmap); x++) {
                bits[FI_RGBA_RED] = 255;
                bits[FI_RGBA_GREEN] = 255;
                bits[FI_RGBA_BLUE] = 255;
                bits[FI_RGBA_ALPHA] = data[x+y*width];
                
                // jump to next pixel
                bits += bytespp;
            }
        }

        /*
        // Draws a gradient from blue to green:
        RGBQUAD color;
        for (unsigned int i=0 ; i < width; i++) {
            for (unsigned int j=0; j < height; j++) {
                unsigned char pixel = data[i+j*width];
                color.rgbRed = pixel;
                color.rgbGreen = pixel;
                color.rgbBlue = pixel;
                FreeImage_SetPixelColor(bitmap, i, height-1-j, &color);
                // Notice how we're calling the & operator on color
                // so that we can pass a pointer to the color struct.
            }
        }
        */
        /*
        int pitch = ((((depth * width) + 31) / 32) * 4);
        bitmap = 
            FreeImage_ConvertFromRawBits(data, width, height, pitch, depth,
                                         FI_RGBA_RED_MASK, 
                                         FI_RGBA_GREEN_MASK, 
                                         FI_RGBA_BLUE_MASK);
        if (!bitmap)
            throw Exception("Error: allocation failed");
        */

        //if ( !FreeImage_Save(FIF_PNG, bitmap, filename.c_str(), PNG_Z_BEST_COMPRESSION) )
        if ( !FreeImage_Save(FIF_PNG, bitmap, filename.c_str(), PNG_Z_BEST_SPEED) )
            throw Exception("Failed to save image: " + filename);
        FreeImage_Unload(bitmap);
        FreeImage_DeInitialise();
    }
};

#endif // _TEXTURE_TOOL_
