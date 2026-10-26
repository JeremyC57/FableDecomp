// Behaviour test for 0x00AFBBD0 (CPSCRender* destructor): both primitive handles are cleared (last-handle notification when
// the reference is unique, then released), each primitive is destroyed only when its count reaches zero.
#include <cstdio>

static int g_notified, g_destroyed, g_baseDtors;
struct CPrimitive {
    long RefCount;
    void Destroy() { ++g_destroyed; }
    void OnLastHandleReleased() { ++g_notified; }
    void Release() { if (--RefCount == 0) Destroy(); }
};
class CEnginePrimitiveHandle { public:
    CPrimitive* Object;
    void Clear()
    {
        if (Object != 0 && Object->RefCount == 1)
            Object->OnLastHandleReleased();
        if (Object) { Object->Release(); Object = 0; }
        Object = 0;
    }
    ~CEnginePrimitiveHandle() { Clear(); if (Object) { Object->Release(); Object = 0; } }
};
struct CBaseClassNonCopyable { virtual ~CBaseClassNonCopyable() { ++g_baseDtors; } };
struct CPSCRender : CBaseClassNonCopyable {
    CEnginePrimitiveHandle PrimHandle; CEnginePrimitiveHandle TrailPrimHandle;
    virtual ~CPSCRender() { PrimHandle.Clear(); TrailPrimHandle.Clear(); }
};
int main()
{
    CPrimitive a = { 1 }, b = { 3 };
    CPSCRender* r = new CPSCRender;
    r->PrimHandle.Object = &a; r->TrailPrimHandle.Object = &b;
    delete r;
    if (g_notified != 1 || g_destroyed != 1 || a.RefCount != 0 || b.RefCount != 2 || g_baseDtors != 1) { std::printf("00afbbd0_TEST FAIL\n"); return 1; }
    CPSCRender* e = new CPSCRender; e->PrimHandle.Object = 0; e->TrailPrimHandle.Object = 0; delete e;
    if (g_notified != 1 || g_baseDtors != 2) { std::printf("00afbbd0_TEST FAIL\n"); return 1; }
    std::printf("00afbbd0_TEST PASS\n");
    return 0;
}