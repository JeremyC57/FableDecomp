// Behaviour test for 00bc5c50: src/00bc5c50.cpp copied verbatim; SetChildPrimitive grows the slot vector, creates the
// child through the type's manager, releases the old child, updates and enables the new one, then refreshes the mask.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CEngineInternalPrimitiveChanger::SetChildPrimitive(unsigned long index, const CEnginePrimitive& primitive, unsigned long flags) -- put a new internal primitive (created by the descriptor type's manager) into child slot `index` (growing the slot vector), release the old one, apply the descriptor, set the child's Enable bit from flag 0x20 being clear, then refresh the render-layer mask
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BC5C50, length 270 bytes (`ret 0Ch`).
// Callees: vector<CEnginePrimitiveHandle>::Resize(long) @ 0x00443F90 (manifest label "CEnginePrimitiveRenderer::AddPrimitive" is
// propagated; landed callers name it Resize), the type manager's virtual slot 1 (+0x04, CreateInternalPrimitive(type)), the old
// child's slots 3/1 (OnLastHandleReleased, Destroy), the new child's slot 5 (+0x14, Update(const CEnginePrimitive&, flags)),
// UpdatePosition @ 0x00B91840 (`__fastcall`). Global: the primitive type info @ 0x01436E84 (engine/CEnginePrimitiveTypeInfo.h:
// PrimitiveManagers[] at +0x10).
// Layout/shape: the landed CEngineInternalPrimitiveChanger::ClearChildPrimitive 0x00BC58E0 model (handle vector +0x48, current
// index +0x58, flags +0x60; inline Clear and the mask helper); the slot is filled by an inline Attach (release the old object,
// store the new one without an add-ref -- the creator's reference is adopted); the Enable bit is computed before the manager
// test (`shr 5 ; not ; and 1`); `this` is parked in the `push ecx` slot.
#include "engine/CEnginePrimitiveTypeInfo.h"
class CEnginePrimitiveHandle;

class CEnginePersistentPrimitiveBase {
public:
    virtual void v000();
    virtual void Destroy();                                          // slot 1 (+0x04)
    virtual void v002();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle); // slot 3 (+0x0C)
    long RefCount;                                                   // +0x04
    void Release()
    {
        if (--RefCount == 0)
            Destroy();
    }
};

class CEngineInternalPrimitiveBase : public CEnginePersistentPrimitiveBase {   // engine/CEngineInternalPrimitiveBase.h (PDB, 0x48 bytes)
public:
    unsigned char _pad08[0x28];
    unsigned long RenderLayerMask;                 // +0x30
    unsigned char Enable : 1;                      // +0x34 bit 0
    unsigned char _pad35[0x13];
    virtual void v004();
    virtual void Update(const class CEnginePrimitive& primitive, unsigned long flags);   // slot 5 (+0x14)
};

class CIVCountedPointer {                          // CIVCountedPointer<CEngineInternalPrimitiveBase>
public:
    CEngineInternalPrimitiveBase* Object;
    CEngineInternalPrimitiveBase* operator->() const { return Object; }
    bool IsUnique() const { return Object != 0 && Object->RefCount == 1; }
    __forceinline void Attach(CEngineInternalPrimitiveBase* object)
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
        Object = object;
    }
    __forceinline CIVCountedPointer& operator=(int)
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
        Object = 0;
        return *this;
    }
    __forceinline CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        CEngineInternalPrimitiveBase* old = Object;
        if (old != other.Object) {
            if (old) {
                old->Release();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->RefCount;
        }
        return *this;
    }
};

class CEnginePrimitiveHandle {                     // 8 bytes: vptr (0x0122F598) + counted Data
public:
    virtual void v000();
    CIVCountedPointer Data;                        // +0x04
    __forceinline void Clear()
    {
        if (Data.IsUnique())
            Data->OnLastHandleReleased(this);
        Data = 0;
    }
};

struct CRetailHandleVector {                       // STLport vector<CEnginePrimitiveHandle>: first, last, end
    CEnginePrimitiveHandle* m_pFirst;
    CEnginePrimitiveHandle* m_pLast;
    CEnginePrimitiveHandle* m_pEnd;
    unsigned long GetCount() const { return m_pLast - m_pFirst; }
    void Resize(long count);                       // 0x00443F90
};

class CEnginePrimitive {                           // 12 bytes: Type, InterpolationSet, ThemeChannelDesc
public:
    long          Type;
    long          InterpolationSet;
    unsigned long ThemeChannelDesc;
};

class CEnginePrimitiveChanger : public CEnginePrimitive {   // PDB, 28 bytes
public:
    long              ActiveIndex;                 // +0x0c
    long              InitialSize;                 // +0x10
    bool              PreloadAll;                  // +0x14
    CIVCountedPointer MeshEffect;                  // +0x18
};

void __fastcall UpdatePosition_00b91840(CEngineInternalPrimitiveBase* primitive, unsigned long flags);   // 0x00B91840

class CEngineInternalPrimitiveChanger : public CEngineInternalPrimitiveBase {   // no PDB layout: handle vector +0x48, current index +0x58, flags +0x60
public:
    CRetailHandleVector Primitives;                // +0x48
    CIVCountedPointer   MeshEffect;                // +0x54 (CIVCountedPointer<CEngineMeshEffect>)
    long                m_currentIndex;            // +0x58 ActiveIndex (-1 = none)
    bool                PreloadAll;                // +0x5c
    unsigned long       m_flags;                   // +0x60
    unsigned long ComputeRenderLayerMask()
    {
        unsigned long mask = 0;
        long index = m_currentIndex;
        if (index != -1 && (unsigned long)index < Primitives.GetCount()) {
            CEngineInternalPrimitiveBase* child = Primitives.m_pFirst[index].Data.Object;
            if (child)
                mask = child->RenderLayerMask;
        }
        return mask;
    }
    void UpdateRenderLayerMask()
    {
        unsigned long mask = ComputeRenderLayerMask();
        if (RenderLayerMask != mask) {
            RenderLayerMask = mask;
            UpdatePosition_00b91840(this, m_flags);
        }
    }
    void SetChildPrimitive(unsigned long index, const CEnginePrimitive& primitive, unsigned long flags);
};

class CEnginePrimitiveManager { public:
    virtual void v00();
    virtual CEngineInternalPrimitiveBase* CreateInternalPrimitive(long type);   // slot 1 (+0x04)
};
extern CEnginePrimitiveTypeInfo* g_pEnginePrimitiveTypeInfo;   // 0x01436E84

void CEngineInternalPrimitiveChanger::SetChildPrimitive(unsigned long index, const CEnginePrimitive& primitive, unsigned long flags)
{
    if (index >= Primitives.GetCount())
        Primitives.Resize(index + 1);
    CEnginePrimitiveHandle& slot = Primitives.m_pFirst[index];
    CEnginePrimitiveManager* manager = g_pEnginePrimitiveTypeInfo->PrimitiveManagers[primitive.Type];
    bool enable = (flags & 0x20) == 0;
    if (!manager)
        return;
    CEngineInternalPrimitiveBase* child = manager->CreateInternalPrimitive(primitive.Type);
    slot.Clear();
    slot.Data.Attach(child);
    child->Update(primitive, flags);
    child->Enable = enable;
    UpdateRenderLayerMask();
}
// ---- end source ----
static int g_destroyed, g_lastReleased, g_updates, g_positions; static unsigned long g_updFlags; static long g_createdType;
void CEnginePersistentPrimitiveBase::v000() {}
void CEnginePersistentPrimitiveBase::Destroy() { ++g_destroyed; }
void CEnginePersistentPrimitiveBase::v002() {}
void CEnginePersistentPrimitiveBase::OnLastHandleReleased(CEnginePrimitiveHandle*) { ++g_lastReleased; }
void CEngineInternalPrimitiveBase::v004() {}
void CEngineInternalPrimitiveBase::Update(const CEnginePrimitive&, unsigned long flags) { ++g_updates; g_updFlags = flags; }
void CEnginePrimitiveHandle::v000() {}
static CEnginePrimitiveHandle g_store[8];
void CRetailHandleVector::Resize(long count) { if (!m_pFirst) m_pFirst = g_store; m_pLast = m_pFirst + count; m_pEnd = m_pFirst + 8; }
static CEngineInternalPrimitiveBase g_children[4]; static int g_next;
struct TestManager : CEnginePrimitiveManager {
    void v00() {}
    CEngineInternalPrimitiveBase* CreateInternalPrimitive(long type) { g_createdType = type; CEngineInternalPrimitiveBase* c = &g_children[g_next++]; c->RefCount = 1; return c; }
};
void CEnginePrimitiveManager::v00() {}
CEngineInternalPrimitiveBase* CEnginePrimitiveManager::CreateInternalPrimitive(long) { return 0; }
void __fastcall UpdatePosition_00b91840(CEngineInternalPrimitiveBase*, unsigned long) { ++g_positions; }
static CEnginePrimitiveTypeInfo g_info;
CEnginePrimitiveTypeInfo* g_pEnginePrimitiveTypeInfo = &g_info;
int main()
{
    TestManager manager;
    memset(&g_info, 0, sizeof(g_info));
    g_info.PrimitiveManagers[3] = &manager;
    for (int i = 0; i < 8; ++i) g_store[i].Data.Object = 0;
    for (int i = 0; i < 4; ++i) { g_children[i].RenderLayerMask = 0x10 << i; g_children[i].Enable = 0; }
    CEngineInternalPrimitiveChanger changer;
    changer.RenderLayerMask = 0; changer.Primitives.m_pFirst = 0; changer.Primitives.m_pLast = 0; changer.Primitives.m_pEnd = 0;
    changer.MeshEffect.Object = 0; changer.m_currentIndex = 1; changer.PreloadAll = false; changer.m_flags = 7;
    CEnginePrimitive prim; prim.Type = 3; prim.InterpolationSet = 0; prim.ThemeChannelDesc = 0;
    changer.SetChildPrimitive(1, prim, 0);
    CHECK(changer.Primitives.GetCount() == 2 && g_createdType == 3);
    CHECK(g_store[1].Data.Object == &g_children[0] && g_children[0].RefCount == 1);
    CHECK(g_updates == 1 && g_updFlags == 0 && g_children[0].Enable == 1);
    CHECK(changer.RenderLayerMask == 0x10 && g_positions == 1);
    // replace slot 1: the old child (unique) gets OnLastHandleReleased and is destroyed; flag 0x20 disables
    changer.SetChildPrimitive(1, prim, 0x20);
    CHECK(g_lastReleased == 1 && g_destroyed == 1);
    CHECK(g_store[1].Data.Object == &g_children[1] && g_children[1].Enable == 0 && g_updFlags == 0x20);
    CHECK(changer.RenderLayerMask == 0x20 && g_positions == 2);
    // slot 0 (not the current index): no mask change, no position update
    changer.SetChildPrimitive(0, prim, 0);
    CHECK(g_store[0].Data.Object == &g_children[2] && changer.Primitives.GetCount() == 2 && g_positions == 2);
    // no manager for the type: nothing happens
    prim.Type = 4; int updates = g_updates;
    changer.SetChildPrimitive(5, prim, 0);
    CHECK(changer.Primitives.GetCount() == 6 && g_updates == updates && g_store[5].Data.Object == 0);
    if (fails == 0) printf("00bc5c50_TEST PASS\n"); else printf("00bc5c50_TEST FAIL\n");
    return fails;
}