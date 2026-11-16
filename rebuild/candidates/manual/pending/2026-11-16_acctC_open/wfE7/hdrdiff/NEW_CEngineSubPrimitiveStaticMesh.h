#ifndef FABLE_ENGINE_CENGINESUBPRIMITIVESTATICMESH_H
#define FABLE_ENGINE_CENGINESUBPRIMITIVESTATICMESH_H
// New header (no repo header for the class; agentC13, row 0x00BA5260, from chunk 2060's model re-based onto the repo's
// CEngineSubPrimitiveMeshBase in engine/CEngineSubPrimitiveRepeatedStaticMesh.h): Ego_r.pdb / FableWin.pdb
// CEngineSubPrimitiveStaticMesh : CEngineSubPrimitiveMeshBase, no own members (struct_layouts_egor 36 bytes; vtable 0x012A41C8, landed
// DrawImmediately 0x00BA6DC0). FableWin.pdb inline members ?GetParent@..@@QAEPAVCEngineInternalPrimitiveStaticMesh@@XZ /
// ?PeekParent@..@@QBEPBV..@@XZ (9 debug instructions: the Parent member cast), ?PeekObjectTransform@..@@QBEABVCMatrix3x4@@XZ (10: the
// parent's ObjectTransform +0x70). FableWin has no MeshPrimitive accessor: the manager reads the protected member directly (friend).
// Needs FABLE_RETAIL_CENGINEINTERNALPRIMITIVESTATICMESH (the retail engine/CEngineInternalPrimitiveStaticMesh.h).
#include "engine/CEngineSubPrimitiveRepeatedStaticMesh.h"
#include "engine/CEngineInternalPrimitiveStaticMesh.h"
class CEnginePrimitiveManagerStaticMeshesBase;
class CEngineSubPrimitiveStaticMesh : public CEngineSubPrimitiveMeshBase {
    friend class CEnginePrimitiveManagerStaticMeshesBase;
public:
#ifdef FABLE_RETAIL_CENGINESUBPRIMITIVESTATICMESH_PARENTBASE
    // Opt-in (bwf2 / agentC16, row 0x00BA2350): the FableWin.pdb GetParent / PeekParent bodies go through GetParentBase / PeekParentBase
    // (9 debug instructions each, COMDAT order); needs FABLE_RETAIL_CENGINESUBPRIMITIVEBASE_ACCESSORS.
    CEngineInternalPrimitiveStaticMesh* GetParent() { return static_cast<CEngineInternalPrimitiveStaticMesh*>(GetParentBase()); }
    const CEngineInternalPrimitiveStaticMesh* PeekParent() const { return static_cast<const CEngineInternalPrimitiveStaticMesh*>(PeekParentBase()); }
#else
    CEngineInternalPrimitiveStaticMesh* GetParent() { return static_cast<CEngineInternalPrimitiveStaticMesh*>(Parent); }
    const CEngineInternalPrimitiveStaticMesh* PeekParent() const { return static_cast<const CEngineInternalPrimitiveStaticMesh*>(Parent); }
#endif
    const CMatrix3x4& PeekObjectTransform() const { return PeekParent()->ObjectTransform; }
};
FABLE_STATIC_ASSERT_AT(20, sizeof(CEngineSubPrimitiveStaticMesh) == 0x24);
#endif
