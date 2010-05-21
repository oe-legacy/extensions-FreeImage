// Texture3D file list resource.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _TEXTURE_3D_FILE_LIST_RESOURCE_H_
#define _TEXTURE_3D_FILE_LIST_RESOURCE_H_

#include <Resources/Texture3D.h>
#include <Resources/IResourcePlugin.h>
#include <Logging/Logger.h>

#include <string>

namespace OpenEngine {
namespace Resources {

using namespace std;

/**
 * Texture 3D File Resource.
 *
 * @class Texture3DFileResource FreeImage.h Resources/FreeImage.h
 */
template <class T>
class Texture3DFileListResource {
public:
    static Texture3DPtr(T) Create(string dir) {
        std::list< std::string > files =
            Directory::ListFiles(dir);
        return Create(files, dir);
    }

    static Texture3DPtr(T) Create(std::list<std::string> files,
                                  std::string dir) {
        vector<Texture2DPtr(T)> texs;
        list<std::string>::iterator itr = files.begin();
        for (; itr != files.end(); itr++) {
            std::string file = dir + "/" + *itr;
            //logger.info << "Loading file: " << file << logger.end;
            Texture2DPtr(T) tex = 
                ResourceManager< Texture2D<T> >::Create(file);
            texs.push_back(tex);
        }
        Texture3DPtr(T) tex3d = Texture3DPtr(T)(new Texture3D<T>(texs));
        return tex3d;
    }
};

/**
 * FreeImage texture resource plug-in.
 *
 * @class FreeImagePlugin FreeImage.h Resources/FreeImage.h
 */
/*
template <class T> 
class Texture3DFileListResourcePlugin
    : public IResourcePlugin<ITexture> {
public:
    Texture3DFileListResourcePlugin() {
        this->AddExtension("exr.3d");
    }

    ITexturePtr CreateResource(string dir) {
        return ITexturePtr
            (new Texture3DFileListResource<T>(dir));
    }
};
*/
} //NS Resources
} //NS OpenEngine

#endif // _TEXTURE_3D_FILE_LIST_RESOURCE_H_
