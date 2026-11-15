#ifndef FABLE_ENGINE_CENGINEINTERNALPRIMITIVEMORPHINFO_H
#define FABLE_ENGINE_CENGINEINTERNALPRIMITIVEMORPHINFO_H
#ifdef FABLE_RETAIL_PASS22_STATIC_MESH_RENDER
// Both original PDB class/field/method records; native parent00BA1140 uses these original owners.
#include "engine/CVectorMap.h"
#include "engine/CEngineTextureDecal.h"
#include "engine/CCompositeSurfaceInfo.h"
#include "stlport/fable_map.h"
#include "fable_string.h"
class C3DMesh2;
class CEngineInternalPrimitiveMorphInfo {
public:
 const unsigned char* GetHiddenMaterials(const C3DMesh2&);
 // Both original PDB signatures and native00BA1490 call00BEC8D0; existing binding agrees.
 void InitialiseHiddenMaterials(const C3DMesh2&);
 CVectorMap<std::pair<long,long>,CEngineTextureDecal> TextureDecals;
 CVectorMap<long,CCompositeSurfaceInfo> CompositeSurfaces;
 std::set<CCharString> HiddenMaterialNames;
 CArray<unsigned char> HiddenMaterials;
};
#endif
#endif
