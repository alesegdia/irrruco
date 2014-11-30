
/* Copyright (C) 2010-2013 Adam 'entity' Krupicka

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Please note that the Irrlicht Engine is based in part on the work of the
  Independent JPEG Group, the zlib and the libPng. This means that if you use
  the Irrlicht Engine in your product, you must acknowledge somewhere in your
  documentation that you've used the IJG code. It would also be nice to mention
  that you use the Irrlicht Engine, the zlib and libPng. See the README files
  in the jpeglib, the zlib and libPng for further informations.
*/


// This file is part of the "irrRenderer".
// For conditions of distribution and use, see copyright notice in irrRenderer.h

#ifndef IQUADSCENENODE_H
#define IQUADSCENENODE_H

#include <irrlicht/irrlicht.h>

namespace irr{
namespace scene{

class IQuadSceneNode : public irr::scene::ISceneNode
{
    public:
        IQuadSceneNode(irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id=-1);
        virtual ~IQuadSceneNode();
        void render();
        void OnRegisterSceneNode();
        const irr::core::aabbox3d<irr::f32>& getBoundingBox() const;
        void setMaterialTexture(irr::u32 layer, irr::video::ITexture* texture);
        void setMaterialType(irr::video::E_MATERIAL_TYPE tehType);
        irr::video::SMaterial& getMaterial(irr::u32 num);
        irr::u32 getMaterialCount() const;

    protected:
        irr::video::SMaterial Material;
        irr::scene::SMeshBuffer* Buffer;
};

}
}

#endif // IQUADSCENENODE_H
