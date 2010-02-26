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
#include <string>
#include <iostream>
#include <fstream>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/extended_type_info.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/split_member.hpp>
#include <Logging/Logger.h>

namespace OpenEngine {
namespace Resources {

using namespace std;

/**
 * FreeImage resource.
 *
 * @class FreeImage FreeImage.h Resources/FreeImage.h
 */
class FreeImage : public Texture2D<unsigned char> {
private:
    bool loaded;
    //int id;                     //!< material identifier
    string filename;            //!< file name
    //unsigned char* data;        //!< binary material data
    //unsigned int width;                  //!< texture width
    //unsigned int height;                 //!< texture height
    //unsigned int depth;                  //!< texture depth/bits

public:

    /**
     * Constructor
     *
     * @param file tga file to load.
     */

    //    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & boost::serialization::base_object<ITextureResource>(*this);
        ar & filename;
    }

 FreeImage() : loaded(false) {
    };

    static bool freeImageLoaded;
    static void InitFreeImage();

    FreeImage(string file);
    ~FreeImage();

    // resource methods
    virtual void Load();
    virtual void Unload();

    // texture resource methods
    /*
    int GetID();
    void SetID(int id);   
    unsigned int GetWidth();
    unsigned int GetHeight();
    unsigned int GetDepth();
    unsigned char* GetData();
    ColorFormat GetColorFormat();
    virtual void ReverseVertecally();
    */
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

} //NS Resources
} //NS OpenEngine

BOOST_CLASS_EXPORT(OpenEngine::Resources::FreeImage)

#endif // _FREE_IMAGE_H_
