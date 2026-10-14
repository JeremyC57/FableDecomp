// CShopItemDef::CShopItemDef() -- base definition constructor, own vtable, the CShopItem member at +0x2c built out of line, Type cleared
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D405A, length 30 bytes.
// Callees: the definition base constructor @ 0x0044C0C0 (shared by the 265-body "CShoveHaPennyRegionDef"
// family: a propagated label for the common def base), CShopItem::CShopItem @ 0x004D4033
// (Quantity 0, PriceMultiplier 1, MaxStock 0, RestockPeriod 1, the three remaining floats 0 --
// the member layout of engine/CShopItemDef.h at +0x2c..+0x48). This class's vtable is 0x0123B644
// (rtti-vtables.tsv: CShopItemDef).
// Shape: base, vtable, the member constructor, then `and dword ptr [esi+28h],0` -- the Type
// (+0x28) is assigned in the body AFTER the member is built, and the `and ..,0` zero store marks
// the size-optimised def module (#pragma optimize("s", on)).
#pragma optimize("s", on)

struct CDefBase {
    virtual void v00();
    unsigned char _body[0x24];              // +0x04..+0x28
    CDefBase();                             // 0x0044C0C0
};

struct CShopItem {                          // +0x2c..+0x48: Quantity, PriceMultiplier, MaxStock, RestockPeriod, ...
    float Quantity;                         // +0x00
    float PriceMultiplier;                  // +0x04
    float MaxStock;                         // +0x08
    long  RestockPeriod;                    // +0x0c
    float RestockPeriodRandomness;          // +0x10
    float SalesPerDay;                      // +0x14
    float SalesPerDayRandomness;            // +0x18
    CShopItem();                            // 0x004D4033
};

struct CShopItemDef : CDefBase {            // vtable 0x0123B644
    long      Type;                         // +0x28
    CShopItem Item;                         // +0x2c
    virtual void v00();
    CShopItemDef();
};

CShopItemDef::CShopItemDef()
    : CDefBase()
    , Item()
{
    Type = 0;
}
