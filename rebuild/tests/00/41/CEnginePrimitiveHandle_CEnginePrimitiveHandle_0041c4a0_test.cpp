#include <cstdio>
class CEnginePrimitiveHandle;
class CEnginePersistentPrimitiveBase {
public:
    virtual void v000();
    virtual void Destroy();                                          // slot 1 (+0x04)
    virtual void v002();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle); // slot 3 (+0x0C)
    long RefCount;                                                   // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
class CIVCountedPointer {                          // CIVCountedPointer<CEnginePersistentPrimitiveBase>
public:
    CEnginePersistentPrimitiveBase* Object;
    CEnginePersistentPrimitiveBase* operator->() const { return Object; }
    bool IsUnique() const { return Object != 0 && Object->RefCount == 1; }
    __forceinline void ReleaseRef() { if (Object) { Object->Release(); Object = 0; } }
    __forceinline CIVCountedPointer& operator=(int) { ReleaseRef(); Object = 0; return *this; }
    __forceinline ~CIVCountedPointer() { ReleaseRef(); }
};
class CBaseClass {                                 // vtable 0x01231710
public:
    virtual ~CBaseClass();                         // 0x0099A2E0
};
class CEnginePrimitiveHandle : public CBaseClass { // vtable 0x0122F598, 8 bytes
public:
    CIVCountedPointer Data;                        // +0x04
    __forceinline void Clear()
    {
        if (Data.IsUnique())
            Data->OnLastHandleReleased(this);
        Data = 0;
    }
    virtual ~CEnginePrimitiveHandle();
};
CEnginePrimitiveHandle::~CEnginePrimitiveHandle()
{
    Clear();
}

static int g_destroyed, g_told, g_base; static CEnginePrimitiveHandle* g_toldHandle;
void CEnginePersistentPrimitiveBase::v000() {}
void CEnginePersistentPrimitiveBase::Destroy() { ++g_destroyed; }
void CEnginePersistentPrimitiveBase::v002() {}
void CEnginePersistentPrimitiveBase::OnLastHandleReleased(CEnginePrimitiveHandle* h) { ++g_told; g_toldHandle = h; }
CBaseClass::~CBaseClass() { ++g_base; }
int main()
{
    CEnginePersistentPrimitiveBase prim; prim.RefCount = 1;
    CEnginePrimitiveHandle* h1 = new CEnginePrimitiveHandle; h1->Data.Object = &prim;
    CEnginePrimitiveHandle* keep = h1;
    delete h1;                                                   // unique: told, then released to 0 -> Destroy
    bool ok = g_told == 1 && g_toldHandle == keep && g_destroyed == 1 && prim.RefCount == 0 && g_base == 1;
    prim.RefCount = 2;
    { CEnginePrimitiveHandle h2; h2.Data.Object = &prim; }      // shared: not told, count 2 -> 1
    ok = ok && g_told == 1 && g_destroyed == 1 && prim.RefCount == 1 && g_base == 2;
    { CEnginePrimitiveHandle h3; h3.Data.Object = 0; }          // empty
    ok = ok && g_told == 1 && g_destroyed == 1 && g_base == 3;
    std::printf(ok ? "0041c4a0_TEST PASS\n" : "0041c4a0_TEST FAIL\n");
    return ok ? 0 : 1;
}