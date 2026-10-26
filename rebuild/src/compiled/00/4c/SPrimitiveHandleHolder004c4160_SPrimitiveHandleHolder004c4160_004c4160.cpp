// SPrimitiveHandleHolder_004c4160::~SPrimitiveHandleHolder_004c4160() -- destroy the CEnginePrimitiveHandle member at +4: tell the primitive it is losing its last handle, drop the counted Data pointer, then the CBaseClass base
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C4160, length 94 bytes. Callees: the primitive's virtual slot 3 (+0x0C, with
// the handle) and slot 1 (+0x04, Destroy when the count reaches zero), CBaseClass::~CBaseClass @ 0x0099A2E0 (tail jump).
// The manifest label (CObject::~CObject) is propagated and wrong: none of the PDB CObject classes holds a primitive
// handle at +4, and the body only destroys the handle member (`lea esi,[ecx+4]`) of a non-polymorphic owner (no vtable
// store of its own); the owner is named by effect. Layout from the PDB: CEnginePrimitiveHandle = vptr (0x0122F598,
// rtti-vtables.tsv) + CIVCountedPointer<CEnginePersistentPrimitiveBase> Data at +4; the pointee's count is at +4.
// Shape: the handle's destructor, the counted pointer's destructor and its `operator=(0)` are `__forceinline` (else
// the owner's destructor collapses to `add ecx,4 ; jmp ??1CEnginePrimitiveHandle`); the last-handle test is an inline
// `IsUnique()` (`Object != 0 && Object->RefCount == 1`) and the call goes through `operator->`, which keeps the object
// in eax and moves it to ecx (a direct member test loads it into ecx); `Data = 0` is the release plus a second
// unconditional zero store, and the member destructor releases a third time; the out-of-line CBaseClass destructor
// becomes the tail jump.
// Leaf: ??1SPrimitiveHandleHolder_004c4160
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
    CEnginePersistentPrimitiveBase* operator->() const { return Object; }
    bool IsUnique() const { return Object != 0 && Object->RefCount == 1; }
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
    virtual ~CBaseClass();                         // 0x0099A2E0 ??1CBaseClass@@UAE@XZ (virtual: slot 0 of 0x01231710)
};

class CEnginePrimitiveHandle : public CBaseClass { // vtable 0x0122F598
public:
    CIVCountedPointer Data;                        // +0x04
    virtual void v001();
    __forceinline ~CEnginePrimitiveHandle()
    {
        if (Data.IsUnique())
            Data->OnLastHandleReleased(this);
        Data = 0;
    }
};

struct SPrimitiveHandleHolder_004c4160 {
    long                   m_key;                  // +0x00
    CEnginePrimitiveHandle m_handle;               // +0x04
    ~SPrimitiveHandleHolder_004c4160();
};

SPrimitiveHandleHolder_004c4160::~SPrimitiveHandleHolder_004c4160()
{
}