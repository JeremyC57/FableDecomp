// CInventoryCategoryDef::operator=(const CInventoryCategoryDef& other) -- compiler-generated copy assignment: CParentDefClassBase::operator=, then Inventory, NumberOfSlots, DrawItemSlots, SelectEmptySlots, WrapHighlightCursor, CategoryName, AllowItemsToFillMoreThanOneSlot, CategoryIdentifier, AddCategoryOnCreate memberwise
// Leaf: ??4CInventoryCategoryDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045BD42, 75 bytes (size-optimised def TU, odd entry), thiscall, `ret 4`.
// Identity: CInventoryCategoryDef::operator=(const CInventoryCategoryDef&) -- proven; evidence: its only caller is the landed CInventoryCategoryDef::Copy 0x0045BD3D
//   (a `jmp` to it: `*this = *static_cast<const CInventoryCategoryDef*>(other)`); RTTI of vtable 0x0123346C gives CInventoryCategoryDef : CParentDefClassBase : CDefClassBase : CDefPointeeBase; the
//   copied offsets and widths are Ego_r.pdb CInventoryCategoryDef's nine members (retail offset = Ego_r offset - 4: the TLC parent def base is 0x3c bytes); FableWin.pdb name set holds
//   ??4CInventoryCategoryDef@@QAEAAV0@ABV0@@Z. Family G059 (seven def copy assignments of nine members; catalogue "CParentDefClassBase::operator=" is propagated).
// Callees: CParentDefClassBase::operator=(const CParentDefClassBase&) 0x00431ED0 (bound under that name).
// Shape: implicit operator= copies each scalar member with integer moves (no x87 for the floats); `return *this` in eax.
#pragma optimize("s", on)
enum EInventoryCategoryIdentifier { EInventoryCategoryIdentifier_Force32 = 0x7fffffff };
// Layout: the def chain is the retail one from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   CParentDefClassBase : CDefClassBase : CDefPointeeBase : CResource : CIVCountedPointeeBase, vtable 0x01230CBC,
//   0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98, operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
class CInventoryCategoryDef : public CParentDefClassBase {
public:
    virtual void Copy(const CDefClassBase* other);
    long Inventory;                          // +0x3c
    long NumberOfSlots;                      // +0x40
    bool DrawItemSlots;                      // +0x44
    bool SelectEmptySlots;                   // +0x45
    bool WrapHighlightCursor;                // +0x46
    unsigned long CategoryName;              // +0x48
    bool AllowItemsToFillMoreThanOneSlot;    // +0x4c
    EInventoryCategoryIdentifier CategoryIdentifier; // +0x50
    bool AddCategoryOnCreate;                // +0x54
};
void CInventoryCategoryDef::Copy(const CDefClassBase* other)
{
    *this = *static_cast<const CInventoryCategoryDef*>(other);
}