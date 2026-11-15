#ifndef FABLE_ENGINE_CENGINEINTERNALPRIMITIVEMORPHEDANIMATEDMESH_H
#define FABLE_ENGINE_CENGINEINTERNALPRIMITIVEMORPHEDANIMATEDMESH_H
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
