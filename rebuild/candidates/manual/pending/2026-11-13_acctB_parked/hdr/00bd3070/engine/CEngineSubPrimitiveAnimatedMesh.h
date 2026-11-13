#ifndef FABLE_ENGINE_CENGINESUBPRIMITIVEANIMATEDMESH_H
#define FABLE_ENGINE_CENGINESUBPRIMITIVEANIMATEDMESH_H
// NEW header (chunk 4010 scout, 2026-11-12; row 0x00BD3070). CEngineSubPrimitiveMeshBase (Ego_r.pdb, 0x24: MeshPrimitive +0x18,
// RenderMode +0x1c, IsCloth +0x20) and CEngineSubPrimitiveAnimatedMesh (Ego_r.pdb, 0x24, no own members) over the shared
// engine/CEngineSubPrimitiveBase.h (0x18). Retail = Ego_r (no container): the landed DrawImmediately 0x00BA6DC0 builds the static twin with
// these offsets, the landed RenderSubPrimitive 0x00BD7110 reads +0x18 / +0x20, RenderMeshPrimitive reads +0x18 (0x00BD318A) and the
// render mode +0x1c (0x00BD3B1A, the 18-entry jump table 0x00BD5C40). Constructors: FableWin.pdb
// ??0CEngineSubPrimitiveMeshBase@@QAE@PAVCEngineInternalPrimitiveBase@@PAVC3DPrimitive2@@W4ERenderMode@@_N@Z and
// ??0CEngineSubPrimitiveAnimatedMesh@@QAE@PAVCEngineInternalPrimitiveAnimatedMesh@@PAVC3DPrimitive2@@W4ERenderMode@@_N@Z.
// Accessors (FableWin.pdb only, inline in retail; debug sizes): GetRenderMode 9i/17b, GetScale 10i/22b (Parent's Scale +0x58),
// PeekParent (MeshBase) 9i/19b, GetParent / PeekParent (AnimatedMesh) 9i/19b, PeekObjectTransform 12i/29b. Bodies FITTED (fwsize).
#ifndef FABLE_RETAIL_CENGINESUBPRIMITIVEBASE_ACCESSORS
#define FABLE_RETAIL_CENGINESUBPRIMITIVEBASE_ACCESSORS
#endif
#include "engine/CEngineSubPrimitiveBase.h"
class C3DPrimitive2;
class CMatrix3x4;
class CEngineInternalPrimitiveMeshBase;
class CEngineInternalPrimitiveAnimatedMesh;
enum ERenderMode;                                                        // MS forward enum (Ego_r.pdb; no enumerator name recovered)
class CEngineSubPrimitiveMeshBase : public CEngineSubPrimitiveBase {
public:
    CEngineSubPrimitiveMeshBase(CEngineInternalPrimitiveBase* parent, C3DPrimitive2* mesh_primitive, ERenderMode mode, bool is_cloth)
        : CEngineSubPrimitiveBase(parent), MeshPrimitive(mesh_primitive), RenderMode(mode), IsCloth(is_cloth) { }
    ERenderMode GetRenderMode() const { return RenderMode; }
    float GetScale() const;                                              // body needs CEngineInternalPrimitiveMeshBase (define in the row)
    const CEngineInternalPrimitiveMeshBase* PeekParent() const { return (const CEngineInternalPrimitiveMeshBase*)PeekParentBase(); }
    C3DPrimitive2* MeshPrimitive;                                        // +0x18
    ERenderMode    RenderMode;                                           // +0x1c
    bool           IsCloth;                                              // +0x20
};
class CEngineSubPrimitiveAnimatedMesh : public CEngineSubPrimitiveMeshBase {
public:
    CEngineSubPrimitiveAnimatedMesh(CEngineInternalPrimitiveAnimatedMesh* parent, C3DPrimitive2* mesh_primitive, ERenderMode mode, bool is_cloth);
    CEngineInternalPrimitiveAnimatedMesh* GetParent() { return (CEngineInternalPrimitiveAnimatedMesh*)GetParentBase(); }
    const CEngineInternalPrimitiveAnimatedMesh* PeekParent() const { return (const CEngineInternalPrimitiveAnimatedMesh*)PeekParentBase(); }
    const CMatrix3x4& PeekObjectTransform() const;                       // body needs CEngineInternalPrimitiveAnimatedMesh (define in the row)
};
FABLE_STATIC_ASSERT(sizeof(CEngineSubPrimitiveMeshBase) == 0x24);
FABLE_STATIC_ASSERT(sizeof(CEngineSubPrimitiveAnimatedMesh) == 0x24);
#endif // FABLE_ENGINE_CENGINESUBPRIMITIVEANIMATEDMESH_H
