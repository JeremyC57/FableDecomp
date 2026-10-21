// Behaviour test for 0041be70: src/0041be70.cpp copied verbatim; a counted primitive stub records the last-handle
// notification (only at count 1, with the handle) and the destroy at zero count; Data must end null.
#include <cstdio>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CEnginePrimitiveHandle::Clear() -- tell the primitive its last handle is going (virtual slot 3, with the handle) when its
// count is exactly 1, then drop the counted Data pointer (`Data = 0`: release a local copy, null store, store the new null)
// Retry 10b: plain compiler-emitted C++ (the catalog copy of this function is an asm_bake); the same model as the inlined
// Clear of the deleting destructor 0x0041C500.
// Leaf: ?Clear@CEnginePrimitiveHandle
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0041BE70, length 57 bytes.
// Callees: the primitive's virtual slot 3 (+0xC) and, at zero count, its virtual destroy (slot 1, +4).
// Layout as the landed constructor @ 0x005BB32E: {vptr, CIVCountedPointer<CEnginePersistentPrimitiveBase> Data +4}.
struct CBaseClass {                                // vtable 0x01231710
    virtual ~CBaseClass();                         // 0x0099A2E0
};

struct CEnginePrimitiveHandle;

struct CIVCountedPointeeBase {
    virtual void v00();
    virtual void Destroy();                        // slot 1
    virtual void v02();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle);   // slot 3
    long m_refCount;                               // +0x04
};

struct CIVCountedPointer {
    CIVCountedPointeeBase* Object;
    CIVCountedPointer() : Object(0) { }
    __forceinline void Release()
    {
        CIVCountedPointeeBase* object = Object;
        if (object) {
            if (--object->m_refCount == 0)
                object->Destroy();
            Object = 0;
        }
    }
    __forceinline CIVCountedPointer& operator=(CIVCountedPointeeBase* object)
    {
        Release();
        Object = object;
        if (object)
            ++object->m_refCount;
        return *this;
    }
    __forceinline ~CIVCountedPointer() { Release(); }
};

struct CEnginePrimitiveHandle : CBaseClass {       // vtable 0x0122F598
    CIVCountedPointer Data;                        // +0x04
    void Clear();
};

void CEnginePrimitiveHandle::Clear()
{
    if (Data.Object && Data.Object->m_refCount == 1)
        Data.Object->OnLastHandleReleased(this);
    Data = 0;
}
// ---- end source ----
static int g_notified = 0, g_destroyed = 0; static CEnginePrimitiveHandle* g_handle = 0;
CBaseClass::~CBaseClass() {}
void CIVCountedPointeeBase::v00() {}
void CIVCountedPointeeBase::Destroy() { ++g_destroyed; }
void CIVCountedPointeeBase::v02() {}
void CIVCountedPointeeBase::OnLastHandleReleased(CEnginePrimitiveHandle* handle) { ++g_notified; g_handle = handle; }
int main()
{
    static unsigned char raw[sizeof(CEnginePrimitiveHandle)];
    CEnginePrimitiveHandle* h = (CEnginePrimitiveHandle*)raw;
    CIVCountedPointeeBase prim;
    // count 1: notify with the handle, then the release drops it to zero and destroys
    prim.m_refCount = 1; h->Data.Object = &prim;
    h->Clear();
    CHECK(g_notified == 1 && g_handle == h && g_destroyed == 1 && prim.m_refCount == 0 && h->Data.Object == 0);
    // count 3: no notification, count drops to 2, no destroy
    prim.m_refCount = 3; h->Data.Object = &prim;
    h->Clear();
    CHECK(g_notified == 1 && g_destroyed == 1 && prim.m_refCount == 2 && h->Data.Object == 0);
    // empty: nothing happens
    h->Clear();
    CHECK(g_notified == 1 && g_destroyed == 1 && h->Data.Object == 0);
    if (fails == 0) printf("0041be70_TEST PASS\n"); else printf("0041be70_TEST FAIL\n");
    return fails;
}