#ifndef _TEXTURE_TOOL_
#define _TEXTURE_TOOL_

#include <Resources/Texture2D.h>
#include <string>

using namespace OpenEngine::Resources;

#include <FreeImage.h>

class TextureTool {
 public:
    static void DumpTexture(ITexture2DPtr tex, std::string filename) {
        FreeImage_Initialise();
        //cout << "FreeImage vesrion: " << FreeImage_GetVersion() << endl;
        //cout << FreeImage_GetCopyrightMessage() << endl << endl;

        const unsigned int width = tex->GetWidth();
        const unsigned int height = tex->GetHeight();
        const unsigned int channels = tex->GetChannels(); 

        if(tex->GetType() != Types::UBYTE)
           throw Exception("not a byte texture");
        unsigned char* data = (unsigned char*)tex->GetVoidDataPtr();

        //for (unsigned int i = 0; i<height*width; i++)
        //  data[i] = 255;

        logger.info << width << "x" << height << ":" << channels << logger.end;

        FIBITMAP* bitmap = NULL;

        // since we are outputting thre 8 bit RGB values
        const unsigned int bpp = 24; 
        bitmap = FreeImage_Allocate(width, height, bpp);
        if (!bitmap)
            throw Exception("Error: allocation failed");

        // Draws a gradient from blue to green:
        RGBQUAD color;
        for (unsigned int i=0 ; i < width; i++) {
            for (unsigned int j=0; j < height; j++) {
                unsigned char pixel = data[i+j*width];
                color.rgbRed = pixel;
                color.rgbGreen = pixel;
                color.rgbBlue = pixel;
                FreeImage_SetPixelColor(bitmap, i, height-j, &color);
                // Notice how we're calling the & operator on color
                // so that we can pass a pointer to the color struct.
            }
        }

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

        if ( !FreeImage_Save(FIF_PNG, bitmap, filename.c_str(), 0) )
            throw Exception("Failed to save image: " + filename);
        FreeImage_DeInitialise();
    }
};

#endif // _TEXTURE_TOOL_
