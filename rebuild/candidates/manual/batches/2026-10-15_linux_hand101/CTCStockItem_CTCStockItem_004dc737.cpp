// CTCStockItem::CTCStockItem(CThing* parent) -- thing-component base, for sale and stealable, not initialised, zero price, empty price-tag primitive handle
// Leaf: ??0CTCStockItem
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DC737, length 62 bytes (size-optimised module: pass-through argument
// push, `and dword ptr [..],0` zero stores).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, CBaseClass::CBaseClass @ 0x0099A2D0 (the handle's base).
// Vtables: 0x01240C1C = CTCStockItem, 0x0122F598 = CEnginePrimitiveHandle (rtti-vtables.tsv).
// Layout: Ego_r.pdb CTCStockItem (engine/CTCStockItem.h, 0x1c bytes): ForSale +0x0c, Stealable +0x0d, Initialised +0x0e,
// Price +0x10, PrimHandle +0x14. The header is a flattened POD (`void* __vftable`), unusable as a polymorphic class, so
// the constructor is modelled locally with the header's names and offsets.
// Shape: init list in declaration order; the handle is a member object with an inline constructor whose data pointer is
// itself a CIVCountedPointer member object (its zero follows the handle vtable, as 0x00BFBBC0); under optimize("s")
// both constructors must be `__forceinline` or the handle's is called out of line.
#pragma optimize("s", on)
struct CThing;

struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CTCBase(CThing* parent);                                // 0x00686800
};

struct CBaseClass {                                         // vtable 0x01231710
    CBaseClass();                                           // 0x0099A2D0
    virtual void v00();
};

struct CEnginePersistentPrimitiveBase;
struct CIVCountedPointer {                                  // CIVCountedPointer<CEnginePersistentPrimitiveBase>
    CEnginePersistentPrimitiveBase* m_pObject;
    __forceinline CIVCountedPointer() : m_pObject(0) { }
};

struct CEnginePrimitiveHandle : CBaseClass {                // vtable 0x0122F598, 8 bytes
    CIVCountedPointer Data;                                 // +0x04
    __forceinline CEnginePrimitiveHandle() { }
    virtual void v00();
};

struct CTCStockItem : CTCBase {
    bool                   ForSale;                         // +0x0c
    bool                   Stealable;                       // +0x0d
    bool                   Initialised;                     // +0x0e
    long                   Price;                           // +0x10
    CEnginePrimitiveHandle PrimHandle;                      // +0x14
    virtual void v00();                                     // own vtable 0x01240C1C
    CTCStockItem(CThing* parent);
};

CTCStockItem::CTCStockItem(CThing* parent)
    : CTCBase(parent)
    , ForSale(true)
    , Stealable(true)
    , Initialised(false)
    , Price(0)
{
}
