// NInventory::CTCInventoryBase::IsSpaceInInventoryForItem(long itemDefIndex) const -- whether the inventory can take one more of the
// item: maps the item definition to its inventory category, finds the category with that index and asks it.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BF2ED, length 32 bytes (tail call). Ego_r.pdb + FableWin.pdb
// ?IsSpaceInInventoryForItem@CTCInventoryBase@NInventory@@QBE_NJ@Z (the catalog's GetItemDescription label here is a propagated
// CItem name; the landed caller 0x00849700 binds this spelling).
// Callees: ?GetInventoryCategory@CTCInventoryItem@@SIJJ@Z @ 0x005D97B0 (static `__fastcall`, the definition index in ecx; Ego_r.pdb +
// FableWin.pdb), the SGI find_if over Categories @ 0x005C08C0 (`__fastcall` template: first in ecx, last in edx, the category index on
// the stack; the landed 0x005BDF08 FindCategoryWithDefIndex model), ?IsSpaceInCategoryForItem@CCategory@NInventory@@QBE_NJ@Z
// @ 0x005BF19C (Ego_r.pdb + FableWin.pdb; GetSlotWithItemsOfType then GetFirstEmptySlot inside).
// Shape: retail does not compare the found category against end -- the result is used directly (the tail `jmp`, which VC7.1 emits
// only with the found category as a named local; folding it into one expression pre-pushes itemDefIndex and calls).
// Layout: retail CTCInventoryBase Categories {first, last, end} at +0x20 (the landed 0x005BDF08 model).
// Leaf: IsSpaceInInventoryForItem
#pragma optimize("s",on)
class CTCInventoryItem { public:
    static long __fastcall GetInventoryCategory(long itemDefIndex);   // 0x005D97B0
};
namespace NInventory {
class CCategory { public:                               // 0x2C bytes
    unsigned char _body[0x2C];
    bool IsSpaceInCategoryForItem(long itemDefIndex) const;   // 0x005BF19C
};
CCategory* __fastcall FindCategoryWithDefIndex(CCategory* first, CCategory* last, long categoryDefIndex);   // 0x005C08C0
struct CCategoryVector {                         // std::vector<CCategory> (12 bytes)
    CCategory* _M_start;
    CCategory* _M_finish;
    CCategory* _M_end_of_storage;
};
class CTCInventoryBase { public:                        // retail 0x130 bytes
    unsigned char   _pad00[0x20];
    CCategoryVector Categories;                  // +0x20
    bool IsSpaceInInventoryForItem(long itemDefIndex) const;
};
bool CTCInventoryBase::IsSpaceInInventoryForItem(long itemDefIndex) const
{
    CCategory* category = FindCategoryWithDefIndex(Categories._M_start, Categories._M_finish,
                                                   CTCInventoryItem::GetInventoryCategory(itemDefIndex));
    return category->IsSpaceInCategoryForItem(itemDefIndex);
}
}
