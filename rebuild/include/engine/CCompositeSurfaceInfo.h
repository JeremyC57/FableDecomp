#ifndef FABLE_ENGINE_CCOMPOSITESURFACEINFO_H
#define FABLE_ENGINE_CCOMPOSITESURFACEINFO_H
#ifdef FABLE_RETAIL_PASS22_STATIC_MESH_RENDER
// Both original PDB class/field/method records; native parent00BA1140 uses these original owners.
#include "engine/CBaseClass.h"
#include "engine/CCountedPointer.h"
#ifdef FABLE_RETAIL_CCOMPOSITESURFACEINFO_MORPH_PASS26
#include "engine/CCompositeSurfaceCache.h"
#include "fable_assert.h"
#endif
class CCompositeSurfaceCache;
class CCompositeSurfaceInfo:public CBaseClass {
public:
#ifdef FABLE_RETAIL_CCOMPOSITESURFACEINFO_MORPH_PASS26
 CEngineTextureMorph& GetCurrentMorph()
 {
     FABLE_RETAIL_ASSERT(Cache.Get());
     return Cache->GetCurrentMorph();
 }
#endif
 CCountedPointer<CCompositeSurfaceCache> Cache;
};
FABLE_STATIC_ASSERT_AT(12, sizeof(CCompositeSurfaceInfo)==12);
#endif
#endif
