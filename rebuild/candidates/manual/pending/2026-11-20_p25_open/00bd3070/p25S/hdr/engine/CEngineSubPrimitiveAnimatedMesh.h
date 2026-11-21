#ifndef FABLE_ENGINE_CENGINESUBPRIMITIVEANIMATEDMESH_H
#define FABLE_ENGINE_CENGINESUBPRIMITIVEANIMATEDMESH_H
#ifdef FABLE_RETAIL_PASS22_ANIMATED_MESH_RENDER
// Layout: original PDB bases/fields and native00BD71B0 operands; no synthetic members.
#include "engine/CEngineSubPrimitiveRepeatedStaticMesh.h"
class CEngineInternalPrimitiveAnimatedMesh;
class CMatrix3x4;
class CEngineSubPrimitiveAnimatedMesh:public CEngineSubPrimitiveMeshBase {
public:
 CEngineSubPrimitiveAnimatedMesh(CEngineInternalPrimitiveAnimatedMesh*,C3DPrimitive2*,ERenderMode,bool);
 virtual ~CEngineSubPrimitiveAnimatedMesh();
#ifdef FABLE_RETAIL_CENGINESUBPRIMITIVEANIMATEDMESH_ACCESSORS
 // Opt-in (wfE7 chunk 4010 scout / p25S, row 0x00BD3070): FableWin.pdb inline ?GetParent@CEngineSubPrimitiveAnimatedMesh@@QAEPAVCEngineInternalPrimitiveAnimatedMesh@@XZ,
 // ?PeekParent@..@@QBEPBVCEngineInternalPrimitiveAnimatedMesh@@XZ (9i/19b, a member read) and ?PeekObjectTransform@..@@QBEABVCMatrix3x4@@XZ (12i/29b; the
 // body needs CEngineInternalPrimitiveAnimatedMesh, so the including row defines it). Needs FABLE_RETAIL_CENGINESUBPRIMITIVEBASE_ACCESSORS.
 CEngineInternalPrimitiveAnimatedMesh* GetParent() { return (CEngineInternalPrimitiveAnimatedMesh*)GetParentBase(); }
 const CEngineInternalPrimitiveAnimatedMesh* PeekParent() const { return (const CEngineInternalPrimitiveAnimatedMesh*)PeekParentBase(); }
 const CMatrix3x4& PeekObjectTransform() const;
#endif
};
FABLE_STATIC_ASSERT(sizeof(CEngineSubPrimitiveAnimatedMesh)==36);
#endif
#endif
