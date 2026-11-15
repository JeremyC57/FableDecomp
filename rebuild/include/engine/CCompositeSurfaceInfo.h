#ifndef FABLE_ENGINE_CCOMPOSITESURFACEINFO_H
#define FABLE_ENGINE_CCOMPOSITESURFACEINFO_H
#ifdef FABLE_RETAIL_PASS22_STATIC_MESH_RENDER
// Both original PDB class/field/method records; native parent00BA1140 uses these original owners.
#include "engine/CBaseClass.h"
#include "engine/CCountedPointer.h"
class CCompositeSurfaceCache;
class CCompositeSurfaceInfo:public CBaseClass {
public:
 CCountedPointer<CCompositeSurfaceCache> Cache;
};
FABLE_STATIC_ASSERT(sizeof(CCompositeSurfaceInfo)==12);
#endif
#endif
