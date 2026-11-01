// Behaviour test for CEngineInternalAttachedPrimitiveInfo::~CEngineInternalAttachedPrimitiveInfo (0x00B3AB10): the handle tells the
// primitive when it holds its last reference (NotifyReleased with the handle), releases it (Delete on the last release), then
// the CBaseClass roots run (handle first, then the info).
#include <cstdio>
#include <cstring>
static char g_log[32];
static int g_logLen;
static void Log(char c) { if (g_logLen < 31) { g_log[g_logLen++] = c; g_log[g_logLen] = 0; } }
class CEnginePrimitiveHandle;
class CEnginePersistentPrimitiveBase {
public:
    CEnginePersistentPrimitiveBase() : IVRefCount(0), NotifiedHandle(0) { }
    virtual ~CEnginePersistentPrimitiveBase() { }
    virtual void Delete() const { Log('d'); }
    virtual void NotifyReleased(const CEnginePrimitiveHandle& handle) { NotifiedHandle = &handle; Log('n'); }
    mutable unsigned long IVRefCount;
    const CEnginePrimitiveHandle* NotifiedHandle;
};
template <class T> class CIVCountedPointer { public:
    T* Object;
    CIVCountedPointer() : Object(0) { }
    T* operator->() const { return Object; }
    unsigned long GetRefCount() const { return Object ? Object->IVRefCount : 0; }
    void Release()
    {
        T* object = Object;
        if (object) {
            if (--object->IVRefCount == 0)
                object->Delete();
            Object = 0;
        }
    }
    void Reset(T* object) { Release(); Object = object; if (object) ++object->IVRefCount; }
    ~CIVCountedPointer() { Release(); }
};
class CBaseClass { public: virtual ~CBaseClass() { Log('b'); } };
class CEnginePrimitiveHandle : public CBaseClass { public:
    CIVCountedPointer<CEnginePersistentPrimitiveBase> Data;
    unsigned long GetReferenceCount() const { return Data.GetRefCount(); }
    void Clear()
    {
        if (GetReferenceCount() == 1)
            Data->NotifyReleased(*this);
        Data.Reset(0);
    }
    virtual ~CEnginePrimitiveHandle() { Clear(); }
};
class CEngineInternalAttachedPrimitiveInfo : public CBaseClass { public:
    CEnginePrimitiveHandle PrimitiveHandle;
};
int main()
{
    CEnginePersistentPrimitiveBase primitive;
    CEngineInternalAttachedPrimitiveInfo* info = new CEngineInternalAttachedPrimitiveInfo;
    info->PrimitiveHandle.Data.Reset(&primitive);
    const CEnginePrimitiveHandle* handle = &info->PrimitiveHandle;
    delete info;                                                 // last reference: notify, delete, two roots
    int fail = std::strcmp(g_log, "ndbb") != 0 || primitive.NotifiedHandle != handle || primitive.IVRefCount != 0;
    g_logLen = 0; g_log[0] = 0;
    CEnginePersistentPrimitiveBase shared;
    shared.IVRefCount = 1;                                       // someone else holds it too
    CEngineInternalAttachedPrimitiveInfo* other = new CEngineInternalAttachedPrimitiveInfo;
    other->PrimitiveHandle.Data.Reset(&shared);
    delete other;
    if (std::strcmp(g_log, "bb") != 0 || shared.IVRefCount != 1 || shared.NotifiedHandle != 0) fail = 1;
    if (fail) { std::printf("00b3ab10_TEST FAIL\n"); return 1; }
    std::printf("00b3ab10_TEST PASS\n");
    return 0;
}