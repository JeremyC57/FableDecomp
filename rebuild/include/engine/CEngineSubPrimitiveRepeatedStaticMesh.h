#ifndef FABLE_ENGINE_CENGINESUBPRIMITIVEREPEATEDSTATICMESH_H
#define FABLE_ENGINE_CENGINESUBPRIMITIVEREPEATEDSTATICMESH_H
// New header (chunk 2028, row 0x00B9A4A0): CEngineSubPrimitiveMeshBase : CEngineSubPrimitiveBase (Ego_r.pdb, 0x24 bytes: MeshPrimitive
// +0x18, RenderMode +0x1C, IsCloth +0x20; fable_types.h; constructor 0x00B98B40 stores them) and CEngineSubPrimitiveRepeatedStaticMesh
// (RTTI vtable 0x012A3E94, no own member). Ego_r.pdb / FableWin.pdb
// ??0CEngineSubPrimitiveRepeatedStaticMesh@@QAE@PAVCEngineInternalPrimitiveRepeatedStaticMesh@@PAVC3DPrimitive2@@W4ERenderMode@@@Z = retail
// 0x00B98BC0 (`ret 0Ch`), called out of line by the row (0x00B9A9E0). ERenderMode: Ego_r.pdb enum; no enumerator name is recovered.
#include "engine/CEngineSubPrimitiveBase.h"
class C3DPrimitive2;
class CEngineInternalPrimitiveRepeatedStaticMesh;
enum ERenderMode;                                                        // MS forward enum (no enumerator name recovered)
#ifdef FABLE_RETAIL_CENGINESUBPRIMITIVEMESHBASE_ACCESSORS
#include "engine/CEngineInternalPrimitiveMeshBase.h"
#endif
class CEngineSubPrimitiveMeshBase : public CEngineSubPrimitiveBase {
public:
    virtual ~CEngineSubPrimitiveMeshBase();
#ifdef FABLE_RETAIL_CENGINESUBPRIMITIVEMESHBASE_ACCESSORS
    // Opt-in (chunk 2060 row 0x00BA5260 / agentC2 row 0x00BB3E80): FableWin.pdb inline ?GetScale@CEngineSubPrimitiveMeshBase@@QBEMXZ (10 debug
    // instructions: the parent's Scale +0x58), ?GetRenderMode@..@@QBE?AW4ERenderMode@@XZ and ?PeekParent@..@@QBEPBVCEngineInternalPrimitiveMeshBase@@XZ
    // (9 = a member read). Needs FABLE_RETAIL_CENGINESUBPRIMITIVEBASE_ACCESSORS (Parent protected) and the retail engine/CEngineInternalPrimitiveMeshBase.h.
    float GetScale() const { return static_cast<const CEngineInternalPrimitiveMeshBase*>(Parent)->Scale; }
    ERenderMode GetRenderMode() const { return RenderMode; }
    const CEngineInternalPrimitiveMeshBase* PeekParent() const { return static_cast<const CEngineInternalPrimitiveMeshBase*>(Parent); }
#endif
#ifdef FABLE_RETAIL_PASS22_STATIC_MESH_RENDER
// Original Ego_r/FableWin TPI and module916 call sites; private evidence in00ba115c.
 CEngineSubPrimitiveMeshBase(CEngineInternalPrimitiveBase*,C3DPrimitive2*,ERenderMode,bool);
#endif

#ifdef FABLE_RETAIL_PASS22_STATIC_MESH_RENDER
// Both original PDB field lists: MeshPrimitive/RenderMode/IsCloth are public.
public:
#else
protected:
#endif
    C3DPrimitive2* MeshPrimitive;                                        // +0x18
    ERenderMode    RenderMode;                                           // +0x1c
    bool           IsCloth;                                              // +0x20
};
class CEngineSubPrimitiveRepeatedStaticMesh : public CEngineSubPrimitiveMeshBase {
public:
    CEngineSubPrimitiveRepeatedStaticMesh(CEngineInternalPrimitiveRepeatedStaticMesh* parent, C3DPrimitive2* primitive, ERenderMode mode);   // 0x00B98BC0
    virtual ~CEngineSubPrimitiveRepeatedStaticMesh();
};
FABLE_STATIC_ASSERT_AT(25, sizeof(CEngineSubPrimitiveRepeatedStaticMesh) == 0x24);
#endif
