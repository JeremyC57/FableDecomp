// CTCStockItem::~CTCStockItem() -- destroy the PrimHandle CEnginePrimitiveHandle member (tell the primitive it loses its last handle, drop the counted pointer, CBaseClass base), then tail-call the CTCBase destructor
// Leaf: ??1CTCStockItem
// Retail: Fable.exe 0x004DE44D, 101 bytes (size TU: odd start, `and dword ptr [m],0`; base destructor as `jmp`).
// Identity: the manifest label "CTCShotDefinitionProjectile::~CTCShotDefinitionProjectile" is a byte-twin propagation;
// this body is called only by 0x004DE431 = slot 0 (deleting dtor) of vtable 0x01240C1C = CTCStockItem, whose ctor
// 0x004DC737 builds the handle at +0x14. No own vtable store: modelled `__declspec(novtable)`.
// Callees: the primitive's virtual slot 3 (+0x0C) and slot 1 (+0x04), CBaseClass::~CBaseClass @ 0x0099A2E0,
// CTCBase::~CTCBase @ 0x00686830.
// Layout = Ego_r.pdb CTCStockItem (0x1C): ForSale +0x0C, Stealable +0x0D, Initialised +0x0E, Price +0x10, PrimHandle
// (CEnginePrimitiveHandle, vtable 0x0122F598, counted Data +4) +0x14. Handle model = landed 0x00BDFD80 (graphics layer) with the size TU's
// __forceinline GetRefCount() ternary (`Object ? RefCount : 0` == 1).
#pragma optimize("s", on)
class CEnginePrimitiveHandle;
class CEnginePersistentPrimitiveBase {
public:
    virtual void v000();
    virtual void Destroy();                                          // slot 1 (+0x04)
    virtual void v002();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle); // slot 3 (+0x0C)
    long RefCount;                                                   // +0x04
    __forceinline void Release()
    {
        if (--RefCount == 0)
            Destroy();
    }
};
class CIVCountedPointer {
public:
    CEnginePersistentPrimitiveBase* Object;
    CEnginePersistentPrimitiveBase* operator->() const { return Object; }
    __forceinline long GetRefCount() const { return Object ? Object->RefCount : 0; }
    __forceinline bool IsUnique() const { return GetRefCount() == 1; }
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
    ~CBaseClass();                                 // 0x0099A2E0 (manifest "CBase::CBase" is propagated)
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
struct CThing;
struct CTCBase {
    virtual ~CTCBase();                            // 0x00686830
    CThing* ParentThing;                           // +0x04
    bool    ToBeRemoved;                           // +0x08
    bool    IsInGlobalUpdateTCs;                   // +0x09
};
struct __declspec(novtable) CTCStockItem : CTCBase {   // vtable 0x01240C1C
    bool                   ForSale;                // +0x0c
    bool                   Stealable;              // +0x0d
    bool                   Initialised;            // +0x0e
    long                   Price;                  // +0x10
    CEnginePrimitiveHandle PrimHandle;             // +0x14
    ~CTCStockItem();
};

CTCStockItem::~CTCStockItem()
{
}