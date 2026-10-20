// Behaviour test for 0x00441CB0 (record destructor): every handle in the four handle vectors and the embedded handle is
// destroyed (last-handle notification when unique, reference released), the vector blocks and the POD block are freed.
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_lastNotified, g_destroyed, g_handleDtors, g_freed;
static void Free(void* p) { ++g_freed; std::free(p); }
struct CPrimitive {
    long RefCount;
    void Destroy() { ++g_destroyed; }
    void OnLastHandleReleased() { ++g_lastNotified; }
    void Release() { if (--RefCount == 0) Destroy(); }
};
struct CEnginePrimitiveHandle {
    CPrimitive* Object;
    void Clear()
    {
        if (Object != 0 && Object->RefCount == 1)
            Object->OnLastHandleReleased();
        if (Object) { Object->Release(); Object = 0; }
        Object = 0;
    }
    virtual ~CEnginePrimitiveHandle() { Clear(); if (Object) { Object->Release(); Object = 0; } ++g_handleDtors; }
};
struct CVec {
    CEnginePrimitiveHandle* s; CEnginePrimitiveHandle* f; CEnginePrimitiveHandle* e;
    void Init(CPrimitive* prims, int n)
    {
        s = n ? (CEnginePrimitiveHandle*)std::malloc(n * sizeof(CEnginePrimitiveHandle)) : 0; f = s; e = s + n;
        for (int i = 0; i < n; ++i) { new (f) CEnginePrimitiveHandle(); f->Object = &prims[i]; ++prims[i].RefCount; ++f; }
    }
    ~CVec() { for (CEnginePrimitiveHandle* p = s; p != f; ++p) p->~CEnginePrimitiveHandle(); if (s) Free(s); }
};
struct CPOD { void* s; ~CPOD() { if (s) Free(s); } };
struct CRecord { CVec Handles1c; CEnginePrimitiveHandle Handle5c; CVec Handles64, Handles70, Handles7c; CPOD Data88; };
int main()
{
    CPrimitive prims[6] = { {0}, {0}, {0}, {0}, {1}, {0} };
    prims[1].RefCount = 1;                       // shared with someone else: no notification
    {
        CRecord* r = new CRecord;
        r->Handles1c.Init(&prims[0], 2);
        r->Handle5c.Object = &prims[4];          // unique (count 1)
        r->Handles64.Init(0, 0);
        r->Handles70.Init(&prims[2], 1);
        r->Handles7c.Init(&prims[3], 1);
        r->Data88.s = std::malloc(16);
        delete r;
    }
    if (g_handleDtors != 5 || g_freed != 4) { std::printf("00441cb0_TEST FAIL\n"); return 1; }
    if (g_lastNotified != 4 || g_destroyed != 4 || prims[1].RefCount != 1) { std::printf("00441cb0_TEST FAIL\n"); return 1; }
    std::printf("00441cb0_TEST PASS\n");
    return 0;
}
