// CEnginePrimitiveHandle::SetData(CEnginePersistentPrimitiveBase*) -- clear the handle (telling a uniquely held primitive that its last handle is going) and adopt the given primitive pointer
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B38780, 99 bytes (+ int3 padding; `ret 4`).
// Label correction: the catalog calls this CObject::~CObject (propagated); a destructor takes no argument. The body is the
//   CEnginePrimitiveHandle Clear() of the landed 0x00BBC310 model followed by a raw-pointer assignment to Data (no AddRef: the
//   caller hands over its reference). The real member name is not in the PDB symbol list; SetData is descriptive.
// Callees: the primitive's virtual slots 3 OnLastHandleReleased (+0xc) and 1 Destroy (+4).
// Shape: Clear() = `if (Data.IsUnique()) Data->OnLastHandleReleased(this); Data = 0;` (release with the zero store inside the
//   null test, then the second unconditional zero store), then `Data = p` = the same release followed by the store of p.
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

class CIVCountedPointer {                          // CIVCountedPointer<CEnginePersistentPrimitiveBase>
public:
    CEnginePersistentPrimitiveBase* Object;
    CEnginePersistentPrimitiveBase* operator->() const { return Object; }
    bool IsUnique() const { return Object != 0 && Object->RefCount == 1; }
    __forceinline CIVCountedPointer& operator=(int)
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
        Object = 0;
        return *this;
    }
    __forceinline CIVCountedPointer& operator=(CEnginePersistentPrimitiveBase* object)
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
        Object = object;
        return *this;
    }
};

class CEnginePrimitiveHandle {                     // 8 bytes: vptr (0x0122F598) + counted Data
public:
    virtual void v000();
    CIVCountedPointer Data;                        // +0x04
    __forceinline void Clear()
    {
        if (Data.IsUnique())
            Data->OnLastHandleReleased(this);
        Data = 0;
    }
    void SetData(CEnginePersistentPrimitiveBase* object);
};

void CEnginePrimitiveHandle::SetData(CEnginePersistentPrimitiveBase* object)
{
    Clear();
    Data = object;
}