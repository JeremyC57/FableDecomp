// Behaviour test for 004dd728: pair<const long, CEnginePrimitiveHandle>::~pair -- the handle at +4 tells the primitive about its last handle and drops the counted pointer
#include <cstdio>
class CEnginePrimitiveHandle;
static int g_destroyed = 0, g_lastHandle = 0;
class CEnginePersistentPrimitiveBase {
public:
    long RefCount;
    CEnginePersistentPrimitiveBase() : RefCount(0) { }
    virtual ~CEnginePersistentPrimitiveBase() { }
    virtual void Destroy() { ++g_destroyed; }
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle*) { ++g_lastHandle; }
    void Release() { if (--RefCount == 0) Destroy(); }
};
class CIVCountedPointer {
public:
    CEnginePersistentPrimitiveBase* Object;
    long GetRefCount() const { return Object ? Object->RefCount : 0; }
    ~CIVCountedPointer() { if (Object) { Object->Release(); Object = 0; } }
    CIVCountedPointer& operator=(int) { if (Object) { Object->Release(); Object = 0; } Object = 0; return *this; }
};
class CEnginePrimitiveHandle {
public:
    CIVCountedPointer Data;
    ~CEnginePrimitiveHandle()
    {
        if (Data.GetRefCount() == 1)
            Data.Object->OnLastHandleReleased(this);
        Data = 0;
    }
};
struct PairConstLongHandle { long first; CEnginePrimitiveHandle second; };
int main()
{
    CEnginePersistentPrimitiveBase prim; prim.RefCount = 1;
    { PairConstLongHandle h; h.first = 1; h.second.Data.Object = &prim; }
    if (g_lastHandle != 1 || g_destroyed != 1 || prim.RefCount != 0) { std::printf("004dd728_TEST FAIL\n"); return 1; }
    prim.RefCount = 2;
    { PairConstLongHandle h; h.first = 2; h.second.Data.Object = &prim; }
    if (g_lastHandle != 1 || g_destroyed != 1 || prim.RefCount != 1) { std::printf("004dd728_TEST FAIL\n"); return 1; }
    { PairConstLongHandle h; h.first = 3; h.second.Data.Object = 0; }
    std::printf("004dd728_TEST PASS\n");
    return 0;
}