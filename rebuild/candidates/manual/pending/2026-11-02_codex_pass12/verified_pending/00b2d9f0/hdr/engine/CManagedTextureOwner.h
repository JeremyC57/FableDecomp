#ifdef FABLE_RETAIL_TEXTURE_CACHE
#ifndef FABLE_ENGINE_CMANAGEDTEXTUREOWNER_H
#define FABLE_ENGINE_CMANAGEDTEXTUREOWNER_H
#include "engine/CResource.h"
// Original sole base and added slot10; no added storage.
class CManagedTextureOwner:public CResource {public:virtual ~CManagedTextureOwner();virtual void ReleaseTexture()=0;};
FABLE_STATIC_ASSERT(sizeof(CManagedTextureOwner)==0x1c);
#endif

#else
#ifndef FABLE_ENGINE_CMANAGEDTEXTUREOWNER_H
#define FABLE_ENGINE_CMANAGEDTEXTUREOWNER_H
#include "engine/CResource.h"
// Original sole base and added slot10; no added storage.
class CManagedTextureOwner:public CResource {public:virtual ~CManagedTextureOwner();virtual void ReleaseTexture();};
FABLE_STATIC_ASSERT(sizeof(CManagedTextureOwner)==0x1c);
#endif

#endif
