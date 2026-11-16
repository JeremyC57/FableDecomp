#ifndef FABLE_ENGINE_CENGINEINTERNALPRIMITIVEMORPHEDANIMATEDMESH_H
#define FABLE_ENGINE_CENGINEINTERNALPRIMITIVEMORPHEDANIMATEDMESH_H
#ifdef FABLE_RETAIL_PASS22_ANIMATED_MESH_RENDER
// Layout: original PDB bases/fields and native00BD71B0 operands; no synthetic members.
#include "engine/CEngineInternalPrimitiveAnimatedMesh.h"
#include "engine/CEngineInternalPrimitiveMorphInfo.h"
class CEngineInternalPrimitiveMorphedAnimatedMesh:public CEngineInternalPrimitiveAnimatedMesh {
public: CEngineInternalPrimitiveMorphInfo Morph;
};
FABLE_STATIC_ASSERT(sizeof(CEngineInternalPrimitiveMorphedAnimatedMesh)==0x160);
#else
// New header (wfA6b, row 0x00BD5C90 CEnginePrimitiveManagerAnimatedMeshesBase::RenderPrimitiveToShadowBuffer). Ego_r.pdb / FableWin.pdb
// CEngineInternalPrimitiveMorphedAnimatedMesh : CEngineInternalPrimitiveAnimatedMesh with Morph (CEngineInternalPrimitiveMorphInfo) right
// after the retail-sized base (+0x128: the row reads the hidden-material names count at +0x14C and takes &Morph.CompositeSurfaces
// = +0x138, 0x00BD5F4C..0x00BD5F8C); ?GetMorphInfo@CEngineInternalPrimitiveMorphedAnimatedMesh@@UAEPAVCEngineInternalPrimitiveMorphInfo@@XZ
// (virtual). The morph-info class is engine/CEngineInternalPrimitiveMorphInfo.h. Needs the retail-sized base
// (FABLE_RETAIL_CENGINEINTERNALPRIMITIVEANIMATEDMESH_ORIGINAL_CACHE).
#include "engine/CEngineInternalPrimitiveAnimatedMesh.h"
#include "engine/CEngineInternalPrimitiveMorphInfo.h"
class CEngineInternalPrimitiveMorphedAnimatedMesh : public CEngineInternalPrimitiveAnimatedMesh {
public:
    virtual CEngineInternalPrimitiveMorphInfo* GetMorphInfo();
    CEngineInternalPrimitiveMorphInfo Morph;                             // +0x128
};
#endif
#endif
