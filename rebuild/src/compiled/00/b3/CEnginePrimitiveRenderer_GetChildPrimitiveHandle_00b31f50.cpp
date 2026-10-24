// CEnginePrimitiveRenderer::GetChildPrimitiveHandle(CEnginePrimitiveHandle& handle, unsigned long id, CEnginePrimitiveHandle& childHandle) -- fetches the handle of child `id` of the handle's internal primitive (no-op for an empty handle)
// Leaf: GetChildPrimitiveHandle
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B31F50, length 29 bytes (`ret 0Ch`, `this` unused).
// Signature from Ego_r.pdb: ?GetChildPrimitiveHandle@CEnginePrimitiveRenderer@@QAEXAAVCEnginePrimitiveHandle@@K0@Z;
// reached from CEngine::GetChildPrimitiveHandle @ 0x00B23C40 (`mov ecx,[g_pEnginePrimitiveRenderer 0x01436E80] ; jmp`).
// Callees: CEngineInternalPrimitiveBase vtable slot 9 (+0x24, GetChildPrimitiveHandle(id, handle); base body @ 0x00B8FDC0).
// Shape: two forwarded arguments re-pushed right to left through eax; one shared `ret 0Ch`.
// Model: engine/CEngineInternalPrimitiveBase.h carries the vptr as a data member, so the virtually-called primitive
// classes are modelled locally.
#include "engine/CEnginePrimitiveRenderer.h"

class CEnginePrimitive;
class CEnginePrimitiveHandle;
class CEnginePrimitiveAnimation;
class CEnginePrimitivePosition;

class CEnginePersistentPrimitiveBase {
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    long RefCount;                                   // +0x04
    long Type;                                       // +0x08
};

class CEngineInternalPrimitiveBase : public CEnginePersistentPrimitiveBase {
public:
    virtual unsigned long AddChildPrimitive(const CEnginePrimitive& primitive, unsigned long flags);                                  // slot 6  (+0x18)
    // VC7.1 lays overloaded virtuals out in REVERSE declaration order: declared (id, primitive, flags) first so that
    // (id, handle, primitive, flags) takes slot 7 (+0x1C) and (id, primitive, flags) slot 8 (+0x20), as in the PDB.
    virtual void SetChildPrimitive(unsigned long id, const CEnginePrimitive& primitive, unsigned long flags);                          // slot 8  (+0x20)
    virtual void SetChildPrimitive(unsigned long id, CEnginePrimitiveHandle& handle, const CEnginePrimitive& primitive, unsigned long flags); // slot 7  (+0x1C)
    virtual void GetChildPrimitiveHandle(unsigned long id, CEnginePrimitiveHandle& handle);                                          // slot 9  (+0x24)
    virtual void ClearChildPrimitive(unsigned long id);                                                                               // slot 10 (+0x28)
    virtual void ClearAllChildPrimitives();                                                                                           // slot 11 (+0x2C)
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void SetAnimation(const CEnginePrimitiveAnimation& animation);                                                            // slot 23 (+0x5C)
    virtual void SetPosition(const CEnginePrimitivePosition& position);                                                               // slot 24 (+0x60)

    unsigned long EnginePrimitiveFlags;              // +0x0C
    float         AlphaMipBias;                      // +0x10
    float         FadeBeginDistance;                 // +0x14
    float         MaxDrawDistance;                   // +0x18
    unsigned long OcclusionLayerBitMask;             // +0x1C
    void*         GridCell;                          // +0x20
    signed char   GridPos_Level;                     // +0x24 CEngineSceneGridPos::Level (-1 = not in the grid)

    bool IsInGrid() const { return GridPos_Level >= 0; }
};

class CEngineScene {
public:
    static void __fastcall UpdatePosition(CEngineInternalPrimitiveBase& primitive, unsigned long flags);   // 0x00B91840
};

class CEnginePrimitiveHandle {                       // 8 bytes: vptr + CIVCountedPointer<CEnginePersistentPrimitiveBase> Data
public:
    virtual void v00();
    CEnginePersistentPrimitiveBase* Data;            // +0x04
    CEngineInternalPrimitiveBase* GetInternalPrimitive() const { return static_cast<CEngineInternalPrimitiveBase*>(Data); }
};

struct CEnginePrimitiveRenderer_Methods : CEnginePrimitiveRenderer {
    void GetChildPrimitiveHandle(CEnginePrimitiveHandle& handle, unsigned long id, CEnginePrimitiveHandle& childHandle);
};

void CEnginePrimitiveRenderer_Methods::GetChildPrimitiveHandle(CEnginePrimitiveHandle& handle, unsigned long id, CEnginePrimitiveHandle& childHandle)
{
    CEngineInternalPrimitiveBase* internal = handle.GetInternalPrimitive();
    if (internal)
        internal->GetChildPrimitiveHandle(id, childHandle);
}