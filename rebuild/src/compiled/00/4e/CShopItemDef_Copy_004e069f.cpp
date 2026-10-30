// CShopItemDef::Copy(const CDefClassBase* source) -- the virtual def copy: the definition base's assignment (out of line), then Type and the seven-dword CShopItem block from the source cast to this class
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E069F, length 36 bytes.
// Leaf: Copy
// Callees: the definition base's operator= @ 0x00431F10 (the landed CCardDef::Copy 0x0067C980 calls the same base).
// Layout: engine/CShopItemDef.h (Type +0x28, the CShopItem fields flattened at +0x2c..+0x44). Retail copies the
// CShopItem as ONE 28-byte block (`push 7 ; pop ecx ; rep movsd`), so the seven flattened fields are viewed through a
// local CShopItem record (the member type the header names in its comments).
// Shape: the source loaded into esi before the base call and kept, `this` in edi; `push 7 ; pop ecx` = /Os.
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CShopItemDef.h"


class CShopItem { public:                              // CShopItemDef::Item (+0x2c), 0x1c bytes
    float Quantity;
    float PriceMultiplier;
    float MaxStock;
    long  RestockPeriod;
    float RestockPeriodRandomness;
    float SalesPerDay;
    float SalesPerDayRandomness;
};

#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)

struct CShopItemDef_Methods : CShopItemDef {
    CShopItem&       Item()       { return *reinterpret_cast<CShopItem*>(&Item_Quantity); }
    const CShopItem& Item() const { return *reinterpret_cast<const CShopItem*>(&Item_Quantity); }
    CDefClassBase&       Base()       { return *reinterpret_cast<CDefClassBase*>(this); }
    const CDefClassBase& Base() const { return *reinterpret_cast<const CDefClassBase*>(this); }
    void Copy(const CDefClassBase* source);
};

void CShopItemDef_Methods::Copy(const CDefClassBase* source)
{
    const CShopItemDef_Methods* other = reinterpret_cast<const CShopItemDef_Methods*>(source);
    Base() = other->Base();
    Type = other->Type;
    Item() = other->Item();
}