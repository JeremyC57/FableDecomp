#ifndef FABLE_ENGINE_CENGINESUBPRIMITIVEMESHBASE_H
#define FABLE_ENGINE_CENGINESUBPRIMITIVEMESHBASE_H
// New header (chunk 2060, row 0x00BA5260 CEnginePrimitiveManagerStaticMeshesBase::RenderMeshPrimitive). Ego_r.pdb
// CEngineSubPrimitiveMeshBase : CEngineSubPrimitiveBase, 0x24 bytes: MeshPrimitive +0x18, RenderMode +0x1C, IsCloth +0x20
// (struct_layouts_egor.tsv; the landed DrawImmediately 0x00BA6DC0 / RenderSubPrimitive 0x00BA6FA0 read the same offsets).
// FableWin.pdb inline members: ?GetScale@CEngineSubPrimitiveMeshBase@@QBEMXZ (10 debug instructions: the parent's Scale +0x58),
// ?GetRenderMode@..@@QBE?AW4ERenderMode@@XZ, ?PeekParent@..@@QBEPBVCEngineInternalPrimitiveMeshBase@@XZ (9 = a member read);
// ctor ??0CEngineSubPrimitiveMeshBase@@QAE@PAVCEngineInternalPrimitiveBase@@PAVC3DPrimitive2@@W4ERenderMode@@_N@Z.
// Needs FABLE_RETAIL_CENGINESUBPRIMITIVEBASE_ACCESSORS and the retail engine/CEngineInternalPrimitiveMeshBase.h.
#include "engine/CEngineSubPrimitiveBase.h"
#include "engine/CEngineInternalPrimitiveMeshBase.h"
class C3DPrimitive2;
enum ERenderMode;
class CEngineSubPrimitiveMeshBase : public CEngineSubPrimitiveBase {
public:
    CEngineSubPrimitiveMeshBase(CEngineInternalPrimitiveBase* parent, C3DPrimitive2* mesh_primitive, ERenderMode render_mode, bool is_cloth)
        : CEngineSubPrimitiveBase(parent), MeshPrimitive(mesh_primitive), RenderMode(render_mode), IsCloth(is_cloth) { }
    float GetScale() const { return static_cast<const CEngineInternalPrimitiveMeshBase*>(Parent)->Scale; }
    ERenderMode GetRenderMode() const { return RenderMode; }
    const CEngineInternalPrimitiveMeshBase* PeekParent() const { return static_cast<const CEngineInternalPrimitiveMeshBase*>(Parent); }
    C3DPrimitive2* MeshPrimitive;                                        // +0x18
    ERenderMode    RenderMode;                                           // +0x1c
    bool           IsCloth;                                              // +0x20
};
FABLE_STATIC_ASSERT(sizeof(CEngineSubPrimitiveMeshBase) == 0x24);
#endif
