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
struct SPrimitiveHandleHolder { long m_key; CEnginePrimitiveHandle m_handle; };
int main()
{
    CEnginePersistentPrimitiveBase prim; prim.RefCount = 1;
    { SPrimitiveHandleHolder h; h.m_key = 1; h.m_handle.Data.Object = &prim; }
    if (g_lastHandle != 1 || g_destroyed != 1 || prim.RefCount != 0) { std::printf("004dd728_TEST FAIL\n"); return 1; }
    prim.RefCount = 2;
    { SPrimitiveHandleHolder h; h.m_key = 2; h.m_handle.Data.Object = &prim; }
    if (g_lastHandle != 1 || g_destroyed != 1 || prim.RefCount != 1) { std::printf("004dd728_TEST FAIL\n"); return 1; }
    { SPrimitiveHandleHolder h; h.m_key = 3; h.m_handle.Data.Object = 0; }
    std::printf("004dd728_TEST PASS\n");
    return 0;
}
