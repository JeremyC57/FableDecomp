// SPrimitiveHandleHolder_004dd728::~SPrimitiveHandleHolder_004dd728() -- size-optimised twin of the landed 0x004C4160: destroys the CEnginePrimitiveHandle member at +4 (tell the primitive it loses its last handle, drop the counted Data pointer, CBaseClass base)
// Leaf: ??1SPrimitiveHandleHolder_004dd728
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DD728, length 90 bytes.
// Callees: the primitive's virtual slot 3 (+0x0C, with the handle) and slot 1 (+0x04, Destroy when the count reaches
// zero), CBaseClass::~CBaseClass @ 0x0099A2E0 (tail jump). Vtable 0x0122F598 = CEnginePrimitiveHandle.
// Label correction: the manifest "CTCShotDefinitionProjectile::~CTCShotDefinitionProjectile" is propagated (that class
// holds its handle at +0x9c, see 0x004DC991); the body destroys only the handle member of a non-polymorphic owner at +4
// (five callers in unrelated owners: 0x004DFC0E, 0x004E1C38, 0x004E35F1, 0x005BBB1C, 0x005BC030), as in 0x004C4160.
// Shape: `and dword ptr [..],0` = a size TU (`#pragma optimize("s", on)`); the last-handle test here is a ref-count
// getter (`Object ? Object->RefCount : 0`, `xor eax,eax` arm) compared with 1, and the virtual call reloads the member
// straight into ecx; `Data = 0` (release plus a second unconditional zero store) and the member destructor (whose null
// test reuses the flags of the `and`) are `__forceinline`.
#pragma optimize("s", on)
class CEnginePrimitiveHandle;

class CEnginePersistentPrimitiveBase {
public:
    virtual void v000();
    virtual void Destroy();                                          // slot 1 (+0x04)
    virtual void v002();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle); // slot 3 (+0x0C)
    long RefCount;                                                   // +0x04
    void Release()
    {
        if (--RefCount == 0)
            Destroy();
    }
};

class CIVCountedPointer {
public:
    CEnginePersistentPrimitiveBase* Object;
    long GetRefCount() const { return Object ? Object->RefCount : 0; }
    __forceinline ~CIVCountedPointer()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
    __forceinline CIVCountedPointer& operator=(int)
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
        Object = 0;
        return *this;
    }
};

class CBaseClass {                                 // vtable 0x01231710
public:
    virtual void v000();
    ~CBaseClass();                                 // 0x0099A2E0
};

class CEnginePrimitiveHandle : public CBaseClass { // vtable 0x0122F598
public:
    CIVCountedPointer Data;                        // +0x04
    virtual void v001();
    __forceinline ~CEnginePrimitiveHandle()
    {
        if (Data.GetRefCount() == 1)
            Data.Object->OnLastHandleReleased(this);
        Data = 0;
    }
};

struct SPrimitiveHandleHolder_004dd728 {
    long                   m_key;                  // +0x00
    CEnginePrimitiveHandle m_handle;               // +0x04
    ~SPrimitiveHandleHolder_004dd728();
};

SPrimitiveHandleHolder_004dd728::~SPrimitiveHandleHolder_004dd728()
{
}
