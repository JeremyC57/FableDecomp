// CStockItemDef::operator=(const CStockItemDef& other) -- compiler-generated copy assignment: CSubDefClassBase::operator=, then DefaultPrice, DefaultIsStealable, CanOnlyBeSoldToPlayer, SellersBanter, BuyersPhrase, ChanceOfBecomingAvailable, CanBeDisplayedInShop memberwise
// Leaf: ??4CStockItemDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E070C, 63 bytes (size-optimised def TU, odd entry), thiscall, `ret 4`.
// Identity: CStockItemDef::operator=(const CStockItemDef&) -- proven; evidence: its only caller is the landed CStockItemDef::Copy 0x004E0707
//   (a `jmp` to it: `*this = *static_cast<const CStockItemDef*>(other)`); RTTI of vtable 0x0123B74C gives CStockItemDef : CSubDefClassBase : CDefClassBase : CDefPointeeBase; the
//   copied offsets and widths are Ego_r.pdb CStockItemDef's seven members (retail offset = Ego_r offset); FableWin.pdb name set holds
//   ??4CStockItemDef@@QAEAAV0@ABV0@@Z. Family G058 (seven def copy assignments; catalogue "CThingBuildingDef::operator=" is propagated).
// Callees: CDefClassBase::operator=(const CDefClassBase&) 0x00431F10 (bound under that name), reached through the inline implicit CSubDefClassBase::operator=.
// Shape: implicit operator= copies each scalar member with integer moves (no x87 for the floats); `return *this` in eax.
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)
#include "engine/CSubDefClassBase.h"                    // vtable 0x01231D54
// def vtable 0x0123B74C: slot 0 0x004D84A6 (implicit inline dtor); real overrides from the def vtable scan
class CStockItemDef : public CSubDefClassBase {
public:
    virtual void Copy(const CDefClassBase* other);
    long DefaultPrice;                       // +0x28
    bool DefaultIsStealable;                 // +0x2c
    bool CanOnlyBeSoldToPlayer;              // +0x2d
    unsigned long SellersBanter;             // +0x30
    unsigned long BuyersPhrase;              // +0x34
    float ChanceOfBecomingAvailable;         // +0x38
    bool CanBeDisplayedInShop;               // +0x3c
    virtual void Transfer(CPersistContext& context);         // slot 18 0x004DE4B2
    virtual unsigned long GetSizeofClass();                  // slot 20 0x004D4110
};
void CStockItemDef::Copy(const CDefClassBase* other)
{
    *this = *static_cast<const CStockItemDef*>(other);
}