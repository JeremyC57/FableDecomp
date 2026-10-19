#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __cdecl
#define __forceinline inline
#define __declspec(x)
#endif
// Behaviour test: the model and logic below are a copy of the src/ file; the callees are test doubles.
// CEngineInternalPrimitive2DViewport::AddChildPrimitive(const CEnginePrimitive&, unsigned long) -- append a child handle, have the primitive type's manager create the internal primitive, store it in the new handle, update it from the description, enable it unless flag 0x20 is set, recompute the viewport's render-layer mask and return the new child index
// Leaf: AddChildPrimitive
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BE2310, 221 bytes (+ int3 padding), `ret 8`, returns the new index.
// Callees: vector<CEnginePrimitiveHandle>::Resize(long) 0x00443F90 (on the child handles +0x94),
//   CEngineInternalPrimitive2DViewport::UpdateRenderLayerMask 0x00BE1D80 (landed); the primitive-type manager table
//   (global 0x01436E84, managers at +0x10 indexed by CEnginePrimitive::Type) slot 1 CreateInternalPrimitive(type); the new
//   primitive's slots 3 OnLastHandleReleased(handle), 1 Destroy, 5 Update(const CEnginePrimitive&, flags).
// Layout: CEngineInternalPrimitiveBase (root CIVCountedPointeeBase {vptr, RefCount +4}; Enable = bit 0 of the +0x34 bit-field
//   byte, Ego_r.pdb agrees); the viewport's child handles at +0x94 (engine/CEngineInternalPrimitive2DViewport.h: Primitives
//   +0x94 as a 16-byte CArray; retail reads a 12-byte first/last/end vector). The header flattens the primitive base, whose
//   virtuals and bit field this body needs, so `this` is a local model with the header's member name.
// Shape: the child handle is a reference local into the vector; storing the new primitive is the landed
//   CEnginePrimitiveHandle model (`Clear()` = last-handle notice + `Data = 0`, then `Data = p`, every helper inline, so
//   two release sequences precede the store); the index is the old size kept in esi for the return; the enable bit is a bool
//   local `(flags & 0x20) ? false : true` computed before the null test (bl).
struct CEnginePrimitive {
    long Type;                                         // +0x00
};

struct CEnginePrimitiveHandle;
struct CEngineInternalPrimitiveBase {
    virtual void v00() {}
    virtual void Destroy() = 0;                            // slot 1 (+0x04)
    virtual void v02() {}
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle) = 0;   // slot 3 (+0x0c)
    virtual void v04() {}
    virtual void Update(const CEnginePrimitive& primitive, unsigned long flags) = 0;   // slot 5 (+0x14)
    long          RefCount;                            // +0x04
    unsigned char _pad08[0x34 - 8];
    unsigned char Enable : 1;                          // +0x34 bit 0
    unsigned char NonPersistent : 1;
    unsigned char OverrideFadeDistance : 1;
    unsigned char PostRenderEnable : 1;
    void Release()
    {
        if (--RefCount == 0)
            Destroy();
    }
};

class CIVCountedPointer {                              // CIVCountedPointer<CEngineInternalPrimitiveBase>
public:
    CEngineInternalPrimitiveBase* Object;
    CEngineInternalPrimitiveBase* operator->() const { return Object; }
    bool IsUnique() const { return Object != 0 && Object->RefCount == 1; }
    CIVCountedPointer& operator=(CEngineInternalPrimitiveBase* p)
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
        Object = p;
        return *this;
    }
};

struct CEnginePrimitiveHandle {                        // 8 bytes: vptr (0x0122F598) + counted Data
    virtual void v00() {}
    CIVCountedPointer Data;                            // +0x04
    void Clear()
    {
        if (Data.IsUnique())
            Data->OnLastHandleReleased(this);
        Data = 0;
    }
};

struct CHandleVector {                                 // vector<CEnginePrimitiveHandle> (12 bytes)
    CEnginePrimitiveHandle* m_pFirst;
    CEnginePrimitiveHandle* m_pLast;
    CEnginePrimitiveHandle* m_pEnd;
    unsigned long size() const { return m_pLast - m_pFirst; }
    void Resize(long count);                           // 0x00443F90
    CEnginePrimitiveHandle& operator[](unsigned long index) { return m_pFirst[index]; }
};

struct CEnginePrimitiveManagerBase {
    virtual void v00() {}
    virtual CEngineInternalPrimitiveBase* CreateInternalPrimitive(long type) = 0;   // slot 1 (+0x04)
};
struct CEnginePrimitiveManagerTable {
    unsigned char                _pad[0x10];
    CEnginePrimitiveManagerBase* Managers[1];          // +0x10, indexed by primitive type
};
extern CEnginePrimitiveManagerTable* g_pPrimitiveManagers;   // 0x01436E84

struct CEngineInternalPrimitive2DViewport : CEngineInternalPrimitiveBase {
    unsigned char _pad38[0x94 - 0x38];
    CHandleVector Primitives;                          // +0x94
    void UpdateRenderLayerMask();                      // 0x00BE1D80
    unsigned long AddChildPrimitive(const CEnginePrimitive& primitive, unsigned long flags);
};

unsigned long CEngineInternalPrimitive2DViewport::AddChildPrimitive(const CEnginePrimitive& primitive, unsigned long flags)
{
    unsigned long index = Primitives.size();
    Primitives.Resize(index + 1);
    CEnginePrimitiveHandle& handle = Primitives[index];
    CEnginePrimitiveManagerBase* manager = g_pPrimitiveManagers->Managers[primitive.Type];
    bool enable = (flags & 0x20) ? false : true;
    if (manager) {
        CEngineInternalPrimitiveBase* child = manager->CreateInternalPrimitive(primitive.Type);
        handle.Clear();
        handle.Data = child;
        child->Update(primitive, flags);
        child->Enable = enable;
        UpdateRenderLayerMask();
    }
    return index;
}

// ---- test doubles and driver ----
static int g_log[64]; static int g_n = 0;
static void Log(int v) { if (g_n < 64) g_log[g_n++] = v; }
struct TestChild : CEngineInternalPrimitiveBase {
    int id; const CEnginePrimitive* updatedWith; unsigned long updateFlags;
    virtual void Destroy() { Log(10 + id); }
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle*) { Log(20 + id); }
    virtual void Update(const CEnginePrimitive& p, unsigned long f) { updatedWith = &p; updateFlags = f; Log(30 + id); }
};
static TestChild g_children[4]; static int g_next = 0; static long g_typeAsked = -1;
struct TestManager : CEnginePrimitiveManagerBase {
    virtual CEngineInternalPrimitiveBase* CreateInternalPrimitive(long type) { g_typeAsked = type; TestChild* c = &g_children[g_next++]; c->RefCount = 1; Log(40); return c; }
};
static TestManager g_manager;
static CEnginePrimitiveManagerTable* MakeTable() { static CEnginePrimitiveManagerBase* words[16]; CEnginePrimitiveManagerTable* t = reinterpret_cast<CEnginePrimitiveManagerTable*>(words); t->Managers[3] = &g_manager; t->Managers[4] = 0; return t; }
CEnginePrimitiveManagerTable* g_pPrimitiveManagers = MakeTable();
static CEnginePrimitiveHandle g_storage[8];
void CHandleVector::Resize(long count) { if (m_pFirst == 0) { m_pFirst = g_storage; m_pLast = g_storage; m_pEnd = g_storage + 8; } m_pLast = m_pFirst + count; Log(50 + count); }
void CEngineInternalPrimitive2DViewport::UpdateRenderLayerMask() { Log(60); }
int main()
{
    for (int i = 0; i < 4; ++i) { g_children[i].id = i; g_children[i].Enable = 0; }
    struct TestViewport : CEngineInternalPrimitive2DViewport { void Destroy() { } void OnLastHandleReleased(CEnginePrimitiveHandle*) { } void Update(const CEnginePrimitive&, unsigned long) { } };
    static TestViewport v;
    v.Primitives.m_pFirst = g_storage; v.Primitives.m_pLast = g_storage + 2; v.Primitives.m_pEnd = g_storage + 8;
    TestChild old; old.id = 3; old.RefCount = 1;
    g_storage[1].Data.Object = &old;
    CEnginePrimitive prim; prim.Type = 3;
    unsigned long r1 = v.AddChildPrimitive(prim, 0x20);
    // old child told it lost its last handle, destroyed; new child created, stored, updated, disabled (flag 0x20), mask recomputed
    bool ok = r1 == 2 && g_typeAsked == 3 && g_children[0].updatedWith == &prim && g_children[0].updateFlags == 0x20 && g_children[0].Enable == 0;
    unsigned long r2 = v.AddChildPrimitive(prim, 0);
    ok = ok && r2 == 3 && g_children[1].Enable == 1 && g_children[1].updateFlags == 0;
    prim.Type = 4;                                     // no manager for this type: nothing created
    int before = g_next;
    v.Primitives.m_pLast = g_storage + 2;
    unsigned long r3 = v.AddChildPrimitive(prim, 0);
    ok = ok && g_next == before;
    if (0) {
        static const int want[] = { 40, 23, 13, 30, 60, 40, 21, 11, 31, 60 };
        ok = ok && g_n == 10;
        for (int i = 0; ok && i < 10; ++i) ok = g_log[i] == want[i];
        ok = ok && g_storage[1].Data.Object == &g_children[1];
    } else {
        static const int want[] = { 53, 40, 30, 60, 54, 40, 31, 60, 53 };
        ok = ok && g_n == 9;
        for (int i = 0; ok && i < 9; ++i) ok = g_log[i] == want[i];
        ok = ok && g_storage[2].Data.Object == &g_children[0] && g_storage[3].Data.Object == &g_children[1];
    }
    if (!ok) { std::printf("00be2310_TEST FAIL\n"); for (int i = 0; i < g_n; ++i) std::printf("%d ", g_log[i]); return 1; }
    std::printf("00be2310_TEST PASS\n");
    return 0;
}
