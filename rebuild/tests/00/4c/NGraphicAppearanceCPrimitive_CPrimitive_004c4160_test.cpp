#include <cstdio>
// Behaviour test for SPrimitiveHandleHolder_004c4160::~SPrimitiveHandleHolder_004c4160 @ 0x004C4160
class CEnginePrimitiveHandle;
static int g_lastHandleCalls = 0, g_destroyCalls = 0, g_baseDtors = 0;
static const CEnginePrimitiveHandle* g_lastHandle = 0;
class CEnginePersistentPrimitiveBase {
public:
    CEnginePersistentPrimitiveBase(long count) : RefCount(count) {}
    virtual ~CEnginePersistentPrimitiveBase() {}
    virtual void Destroy() { ++g_destroyCalls; }
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle) { ++g_lastHandleCalls; g_lastHandle = handle; }
    long RefCount;
    void Release() { if (--RefCount == 0) Destroy(); }
};
class CIVCountedPointer {
public:
    CEnginePersistentPrimitiveBase* Object;
    CEnginePersistentPrimitiveBase* operator->() const { return Object; }
    bool IsUnique() const { return Object != 0 && Object->RefCount == 1; }
    ~CIVCountedPointer() { if (Object) { Object->Release(); Object = 0; } }
    CIVCountedPointer& operator=(int) { if (Object) { Object->Release(); Object = 0; } Object = 0; return *this; }
};
class CBaseClass { public: virtual ~CBaseClass() { ++g_baseDtors; } };
class CEnginePrimitiveHandle : public CBaseClass {
public:
    CIVCountedPointer Data;
    ~CEnginePrimitiveHandle()
    {
        if (Data.IsUnique())
            Data->OnLastHandleReleased(this);
        Data = 0;
    }
};
struct SPrimitiveHandleHolder_004c4160 {
    long m_key;
    CEnginePrimitiveHandle m_handle;
};
int main()
{
    CEnginePersistentPrimitiveBase unique(1), shared(3);
    const CEnginePrimitiveHandle* handleAddress;
    {
        SPrimitiveHandleHolder_004c4160 h; h.m_key = 1; h.m_handle.Data.Object = &unique; handleAddress = &h.m_handle;
    }
    if (g_lastHandleCalls != 1 || g_lastHandle != handleAddress || g_destroyCalls != 1 || unique.RefCount != 0 || g_baseDtors != 1) { std::printf("004c4160_TEST FAIL\n"); return 1; }
    {
        SPrimitiveHandleHolder_004c4160 h; h.m_key = 2; h.m_handle.Data.Object = &shared;
    }
    if (g_lastHandleCalls != 1 || g_destroyCalls != 1 || shared.RefCount != 2 || g_baseDtors != 2) { std::printf("004c4160_TEST FAIL\n"); return 1; }
    {
        SPrimitiveHandleHolder_004c4160 h; h.m_key = 3; h.m_handle.Data.Object = 0;
    }
    if (g_baseDtors != 3) { std::printf("004c4160_TEST FAIL\n"); return 1; }
    std::printf("004c4160_TEST PASS\n");
    return 0;
}