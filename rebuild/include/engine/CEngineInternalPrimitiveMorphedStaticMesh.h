#ifndef FABLE_ENGINE_CENGINEINTERNALPRIMITIVEMORPHEDSTATICMESH_H
#define FABLE_ENGINE_CENGINEINTERNALPRIMITIVEMORPHEDSTATICMESH_H
#ifdef FABLE_RETAIL_PASS22_STATIC_MESH_RENDER
// Both original PDB class/field/method records; native parent00BA1140 uses these original owners.
#include "engine/CEngineInternalPrimitiveStaticMesh.h"
#include "engine/CEngineInternalPrimitiveMorphInfo.h"
class CEngineInternalPrimitiveMorphedStaticMesh:public CEngineInternalPrimitiveStaticMesh {
public:
 CEngineInternalPrimitiveMorphInfo Morph;
};
#else
// New header (chunk 2060, row 0x00BA5260 CEnginePrimitiveManagerStaticMeshesBase::RenderMeshPrimitive). Ego_r.pdb
// CEngineInternalPrimitiveMorphedStaticMesh : CEngineInternalPrimitiveStaticMesh, Morph (CEngineInternalPrimitiveMorphInfo) at +0xAC;
// CEngineInternalPrimitiveMorphInfo: TextureDecals (CVectorMap<pair<long,long>,CEngineTextureDecal>) +0x00, CompositeSurfaces
// (CVectorMap<long,CCompositeSurfaceInfo>) +0x10 in retail (Ego_r 20-byte CVectorMap -> retail 16), then HiddenMaterialNames
// (STLport set) +0x20 and HiddenMaterials (CArray<unsigned char>) +0x2C (the landed DrawImmediately 0x00BA6DC0 reads +0xAC+0x24 /
// +0xAC+0x2C; RenderMeshPrimitive takes &Morph.CompositeSurfaces = +0xBC, 0x00BA53A5). The members after CompositeSurfaces stay
// opaque here (retail sizeof of the morph info 0x38; set 12 bytes, CArray 12). FableWin.pdb
// ?GetMorphInfo@CEngineInternalPrimitiveMorphedStaticMesh@@UAEPAVCEngineInternalPrimitiveMorphInfo@@XZ returns &Morph (virtual).
#include "engine/CEngineInternalPrimitiveStaticMesh.h"
#include "engine/CEngineInternalPrimitiveMorphInfo.h"
class CEngineInternalPrimitiveMorphedStaticMesh : public CEngineInternalPrimitiveStaticMesh {
public:
    virtual CEngineInternalPrimitiveMorphInfo* GetMorphInfo();
    CEngineInternalPrimitiveMorphInfo Morph;                             // +0xac
};
#endif
#endif
