#ifndef FABLE_ENGINE_CENGINEINTERNALPRIMITIVE2DMORPHEDANIMATEDMESH_H
#define FABLE_ENGINE_CENGINEINTERNALPRIMITIVE2DMORPHEDANIMATEDMESH_H
#ifdef FABLE_RETAIL_PASS22_ANIMATED_MESH_RENDER
// Layout: original PDB bases/fields and native00BD71B0 operands; no synthetic members.
#include "engine/CEngineInternalPrimitive2DAnimatedMesh.h"
#include "engine/CEngineInternalPrimitiveMorphInfo.h"
class CEngineInternalPrimitive2DMorphedAnimatedMesh:public CEngineInternalPrimitive2DAnimatedMesh {
public: CEngineInternalPrimitiveMorphInfo Morph;
};
FABLE_STATIC_ASSERT(sizeof(CEngineInternalPrimitive2DMorphedAnimatedMesh)==0x188);
#else
// New header (wfA6b, row 0x00BD5C90 CEnginePrimitiveManagerAnimatedMeshesBase::RenderPrimitiveToShadowBuffer). FableWin.pdb
// ?GetMorphInfo@CEngineInternalPrimitive2DMorphedAnimatedMesh@@UAEPAVCEngineInternalPrimitiveMorphInfo@@XZ (virtual);
// CEngineInternalPrimitive2DMorphedAnimatedMesh : CEngineInternalPrimitive2DAnimatedMesh with Morph right after the retail-sized
// base (+0x150: the row reads +0x174 and takes +0x160, 0x00BD5F99..0x00BD5FD3). Needs FABLE_RETAIL_CENGINEINTERNALPRIMITIVE2DANIMATEDMESH.
#include "engine/CEngineInternalPrimitive2DAnimatedMesh.h"
#include "engine/CEngineInternalPrimitiveMorphInfo.h"
class CEngineInternalPrimitive2DMorphedAnimatedMesh : public CEngineInternalPrimitive2DAnimatedMesh {
public:
    virtual CEngineInternalPrimitiveMorphInfo* GetMorphInfo();
    CEngineInternalPrimitiveMorphInfo Morph;                             // +0x150
};
#endif
#endif
