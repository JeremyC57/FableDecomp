#ifndef FABLE_ENGINE_CENGINESUBPRIMITIVESTATICMESH_H
#define FABLE_ENGINE_CENGINESUBPRIMITIVESTATICMESH_H
// New header (chunk 2060, row 0x00BA5260): Ego_r.pdb / FableWin.pdb CEngineSubPrimitiveStaticMesh : CEngineSubPrimitiveMeshBase (no
// own members; vtable 0x012A41C8, landed DrawImmediately 0x00BA6DC0). FableWin.pdb inline members ?GetParent@..@@QAEPAVCEngineInternalPrimitiveStaticMesh@@XZ
// / ?PeekParent@..@@QBEPBV..@@XZ (9 debug instructions: the Parent member cast), ?PeekObjectTransform@..@@QBEABVCMatrix3x4@@XZ (10: the
// parent's ObjectTransform +0x70). Needs the retail engine/CEngineInternalPrimitiveStaticMesh.h (FABLE_RETAIL_CENGINEINTERNALPRIMITIVESTATICMESH).
#include "engine/CEngineSubPrimitiveMeshBase.h"
#include "engine/CEngineInternalPrimitiveStaticMesh.h"
class CEngineSubPrimitiveStaticMesh : public CEngineSubPrimitiveMeshBase {
public:
    CEngineSubPrimitiveStaticMesh(CEngineInternalPrimitiveStaticMesh* parent, C3DPrimitive2* mesh_primitive, ERenderMode render_mode, bool is_cloth)
        : CEngineSubPrimitiveMeshBase(parent, mesh_primitive, render_mode, is_cloth) { }
    CEngineInternalPrimitiveStaticMesh* GetParent() { return static_cast<CEngineInternalPrimitiveStaticMesh*>(Parent); }
    const CEngineInternalPrimitiveStaticMesh* PeekParent() const { return static_cast<const CEngineInternalPrimitiveStaticMesh*>(Parent); }
    const CMatrix3x4& PeekObjectTransform() const { return PeekParent()->ObjectTransform; }
};
FABLE_STATIC_ASSERT(sizeof(CEngineSubPrimitiveStaticMesh) == 0x24);
#endif
