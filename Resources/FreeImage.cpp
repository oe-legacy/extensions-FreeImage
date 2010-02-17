// Free image resource.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// Modified by Anders Bach Nielsen <abachn@daimi.au.dk> - 21. Nov 2007
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include <Resources/FreeImage.h>
#include <Resources/Exceptions.h>
#include <Resources/File.h>
#include <Utils/Convert.h>

#include <cstring> // includes memcpy

//@todo make a FreeImage.h in a Meta folder
#include <FreeImage.h>

namespace OpenEngine {
namespace Resources {

using OpenEngine::Utils::Convert;

bool FreeImage::freeImageLoaded = false;

FreeImagePlugin::FreeImagePlugin() {
    this->AddExtension("bmp");
    this->AddExtension("gif");
    this->AddExtension("jpeg");
    this->AddExtension("jpg");
    this->AddExtension("lbm");
    this->AddExtension("pcx");
    this->AddExtension("png");
    this->AddExtension("pnm");
    this->AddExtension("tiff");
    this->AddExtension("tif");
    this->AddExtension("tga");
    this->AddExtension("xcf");
    this->AddExtension("xpm");
    this->AddExtension("xv");
}

ITextureResourcePtr FreeImagePlugin::CreateResource(string file) {
    return ITextureResourcePtr(new FreeImage(file));
}

FreeImage::FreeImage(string filename)
    : ITextureResource(), loaded(false), filename(filename) {
}

FreeImage::~FreeImage() {
    if (loaded) {
        delete[] data;
        loaded = false;
    }
}

void FreeImage::Load() {
    if (loaded) return;

    //Automatocally detects the format(from over 20 formats!)
    FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(filename.c_str(),0);

	FIBITMAP* imagen = FreeImage_Load(formato, filename.c_str());
    if (!imagen)
        throw ResourceException("Error loading file: " + filename + ". Description: " + "File not found"); //@todo: apend freeimage getError()
	
    /*
	FIBITMAP* temp = imagen;
	imagen = FreeImage_ConvertTo32Bits(imagen);
	FreeImage_Unload(temp);
    */
	
	width = FreeImage_GetWidth(imagen);
	height = FreeImage_GetHeight(imagen);
	//depth = FreeImage_GetBPP(imagen);
    channels = FreeImage_GetBPP(imagen)/8;

	unsigned char* pixels = (unsigned char*)FreeImage_GetBits(imagen);

    unsigned int numberOfCharsPerColor = channels;
    unsigned int lineWidth = GetWidth() * numberOfCharsPerColor;
    unsigned long size = lineWidth * GetHeight();
    data = new unsigned char[size];
    std::memcpy(data, pixels, size);
	FreeImage_Unload(imagen);
    
    //flip vertecally
    ReverseVertecally();

    // Set format
    switch(this->channels){
    case 1:
        this->format = LUMINANCE;
        break;
    case 3:
        this->format = BGR;
        break;
    case 4:
        this->format = BGRA;
        break;
    default:
        throw Exception("unknown color format");
    }

    loaded = true;
}

void FreeImage::Unload() {
}

void FreeImage::ReverseVertecally() {
    unsigned int lineWidth = GetWidth() * channels;
    unsigned long size = lineWidth * GetHeight();
    unsigned char* tempArr = new unsigned char[size];

    for (unsigned int i=0, j=size-lineWidth; i < size;
	 i+=lineWidth, j-=lineWidth) {
        std::memcpy(&tempArr[j], &data[i], lineWidth);
    }
    delete[] data;
    data = tempArr;
}
    /*
int FreeImage::GetID() {
    return id;
}

void FreeImage::SetID(int id) {
    this->id = id;
}	

unsigned int FreeImage::GetWidth() {
  return width;
}

unsigned int FreeImage::GetHeight() {
    return height;
}

unsigned int FreeImage::GetDepth() {
  return depth;
}

unsigned char* FreeImage::GetData() {
  return data;
}

ColorFormat FreeImage::GetColorFormat() {
    if (depth==32)
        return BGRA;
    else if (depth==24)
        return BGR;
    else if (depth==8)
        return LUMINANCE;
    else
        throw Exception("unknown color depth");
}
*/
} //NS Resources
} //NS OpenEngine
