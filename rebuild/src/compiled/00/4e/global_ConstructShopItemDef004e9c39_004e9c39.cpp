// _global::Construct_ShopItemDef_004e9c39(CShopItemDef*, const CShopItemDef&) -- STLport _Construct: placement copy-construct one CShopItemDef (CDefClassBase base, the item type, the 28-byte CShopItem)
// Leaf: Construct_ShopItemDef_004e9c39
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E9C39, length 42 bytes.
// Callee: CDefClassBase::CDefClassBase(const CDefClassBase&) @ 0x0046644C. Own vtable 0x0123B644 (RTTI: CShopItemDef).
// Shape: /Gr TU (__fastcall: destination in ecx, source in edx; esi/edi kept for the block move). `new (p) T(v)`
// null-tests the placement result, then the implicit copy constructor: the base copy constructor, the vtable store,
// Type (+0x28) and the POD CShopItem (+0x2C, 7 words) as one `rep movsd`. The count is loaded with `push 7 ; pop ecx`,
// the size-optimised idiom (#pragma optimize("s", on)). Under "s" the implicit copy constructor is emitted out of line
// (a bare `push edx ; call`), so the memberwise copy constructor is spelled out and __forceinline.
// Layout from engine/CShopItemDef.h (PDB: Type +0x28, Item_* +0x2C..+0x44, sizeof 0x48); modelled locally because the
// generated header is a flat byte-packed POD (no base class, no virtual), which cannot express the base copy
// constructor. CDefClassBase is 0x25 bytes byte-packed (landed 0x006250B0 model).
// Label correction: the catalog's `_Cons_val<allocator<pair<CCharString,pair<long,CDateAndTime>>>>` is propagated.
#pragma optimize("s", on)
inline void* operator new(unsigned int, void* p) { return p; }

#pragma pack(push, 1)
class CDefClassBase { public:
    virtual void v00();
    unsigned char _body[0x21];                     // +0x04..+0x25
    CDefClassBase(const CDefClassBase& other);     // 0x0046644C
};
#pragma pack(pop)
class CShopItem { public:                                 // 28 bytes, POD
    float Quantity;                                // +0x00
    float PriceMultiplier;                         // +0x04
    float MaxStock;                                // +0x08
    long  RestockPeriod;                           // +0x0c
    float RestockPeriodRandomness;                 // +0x10
    float SalesPerDay;                             // +0x14
    float SalesPerDayRandomness;                   // +0x18
};
class CShopItemDef : public CDefClassBase { public:              // vtable 0x0123B644
    long Type;                                     // +0x28
    CShopItem Item;                                // +0x2c
    virtual void v00();
    __forceinline CShopItemDef(const CShopItemDef& o) : CDefClassBase(o), Type(o.Type), Item(o.Item) { }
};

void __fastcall Construct_ShopItemDef_004e9c39(CShopItemDef* p, const CShopItemDef& value)
{
    new (p) CShopItemDef(value);
}