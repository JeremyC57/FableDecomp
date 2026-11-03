#ifndef FABLE_ENGINE_CMANAGEDTEXTURE_H
#define FABLE_ENGINE_CMANAGEDTEXTURE_H
#include "engine/CBaseClass.h"
#include "engine/CIVCountedPointer.h"
class CManagedTextureData;
// Original classV, native vtable0129c880, Data+4.
class CManagedTexture:public CBaseClass {public:CManagedTexture(){}virtual ~CManagedTexture();bool Release();private:CIVCountedPointer<CManagedTextureData> Data;};
FABLE_STATIC_ASSERT(sizeof(CManagedTexture)==8);
#endif
