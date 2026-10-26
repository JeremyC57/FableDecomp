#include <cstdio>
class CEnginePrimitiveHandle;
static int g_lastReleased = 0, g_destroyed = 0;
class CEnginePersistentPrimitiveBase { public:
    long RefCount;
    virtual ~CEnginePersistentPrimitiveBase() {}
    virtual void Destroy() { ++g_destroyed; }
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle*) { ++g_lastReleased; }
    void Release() { if (--RefCount == 0) Destroy(); }
};
struct CIVCountedPointer {
    CEnginePersistentPrimitiveBase* Object;
    bool IsUnique() const { return Object != 0 && Object->RefCount == 1; }
    void ReleaseRef() { if (Object) { Object->Release(); Object = 0; } }
    CIVCountedPointer& operator=(int) { ReleaseRef(); Object = 0; return *this; }
};
class CEnginePrimitiveHandle { public:
    CIVCountedPointer Data;
    void Clear() { if (Data.IsUnique()) Data.Object->OnLastHandleReleased(this); Data = 0; }
};
struct CViewport {
    CEnginePrimitiveHandle Handle;
    void ClearHandle() { Handle.Clear(); }
};
int main()
{
    CEnginePersistentPrimitiveBase prim; prim.RefCount = 2;
    CViewport v; v.Handle.Data.Object = &prim;
    v.ClearHandle();
    if (v.Handle.Data.Object != 0 || prim.RefCount != 1 || g_lastReleased != 0 || g_destroyed != 0) { std::printf("0053fc90_TEST FAIL\n"); return 1; }
    v.Handle.Data.Object = &prim;
    v.ClearHandle();
    if (v.Handle.Data.Object != 0 || prim.RefCount != 0 || g_lastReleased != 1 || g_destroyed != 1) { std::printf("0053fc90_TEST FAIL\n"); return 1; }
    v.ClearHandle();
    if (g_lastReleased != 1 || g_destroyed != 1) { std::printf("0053fc90_TEST FAIL\n"); return 1; }
    std::printf("0053fc90_TEST PASS\n");
    return 0;
}