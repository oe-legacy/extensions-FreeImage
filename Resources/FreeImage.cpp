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

#include <Resources/ITexture.h>
#include <Resources/Exceptions.h>
#include <Resources/File.h>
#include <Utils/Convert.h>

namespace OpenEngine {
namespace Resources {

using OpenEngine::Utils::Convert;

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
    // return ITextureResourcePtr(new FreeImage<unsigned char>(file));
    return ITextureResourcePtr(new FImage(file));
}

UCharFreeImagePlugin::UCharFreeImagePlugin() {
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

UCharTexture2DPtr UCharFreeImagePlugin::CreateResource(string file) {
    return UCharTexture2DPtr(new FreeImage<unsigned char>(file));
}

FloatFreeImagePlugin::FloatFreeImagePlugin() {
    this->AddExtension("exr");
}

FloatTexture2DPtr FloatFreeImagePlugin::CreateResource(string file) {
    return FloatTexture2DPtr(new FreeImage<float>(file));
}

} //NS Resources
} //NS OpenEngine
