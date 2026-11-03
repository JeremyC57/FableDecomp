#ifndef FABLE_ENGINE_CCOMPOSITESURFACECACHE_H
#define FABLE_ENGINE_CCOMPOSITESURFACECACHE_H
#include "engine/CManagedTextureOwner.h"
#include "engine/CEngineTextureMorph.h"
#include "engine/CGraphicFrame.h"
#include "engine/CManagedTexture.h"
class CEngineSurfaceCompositionManager;
// Original donor members, native two twenty-byte morphs and complete0x68 receiver.
class CCompositeSurfaceCache:public CManagedTextureOwner {public:
 CCompositeSurfaceCache(CEngineSurfaceCompositionManager& owner):Owner(owner),BankFileIndex(0),LastUsedFrame(0){}
 virtual ~CCompositeSurfaceCache();virtual void ReleaseTexture();
 void SetCurrentMorph(const CEngineTextureMorph& morph){CurrentMorph=morph;}
 CEngineTextureMorph& GetCurrentMorph(){return CurrentMorph;}
 const CEngineTextureMorph& GetCurrentMorph()const{return CurrentMorph;}
 bool IsMultipleTextures()const{return CurrentMorph.Blends.size()>1;}
private:CEngineSurfaceCompositionManager& Owner;CEngineTextureMorph CurrentMorph,CachedMorph;unsigned long MipmapLevel;CGraphicFrame GraphicFrame;CManagedTexture ManagedTexture;unsigned long BankFileIndex,LastUsedFrame;double TimeStamp;
};
FABLE_STATIC_ASSERT(sizeof(CCompositeSurfaceCache)==0x68);
#endif
