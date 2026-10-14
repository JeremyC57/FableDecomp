#pragma optimize("y", off)
#pragma optimize("s", on)
// CCategory::SelectItemOfType(long) -- finds the stack of the given type, makes its slot the selected one and tells the inventory (virtual slot 33)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BDEC1, length 71 bytes; size-optimised TU (`push 14h ; pop ecx` divisor, pass-through pushes).
// Callees: find_if instantiation @ 0x005C13E3 (__fastcall, bound-member predicate), CCategory::SetSelectedItemIndex @ 0x005BC479 (landed), CTCInventoryBase virtual slot 33 (+0x84) with (itemType, const tag&).
// Retail CCategory (sizeof 0x2C): engine/CCategory.h is the Ego_r.pdb layout with a 16-byte
// vector<CItem>; the shipped vector is {first, last, end} (12 bytes), so every member after
// ItemsInCategory sits 4 bytes lower than the header says: +0x14 PDef (CDefPointer<const
// CInventoryCategoryDef>), +0x18 PInventory, +0x1C GlobalDefIndex, +0x20 SelectedItemIndex,
// +0x24 HighlightedItemIndex, +0x28 Selected, +0x29 DisplayItems. The header therefore cannot
// model `this` here; the local CCategory_Retail view below carries the PDB names at the retail offsets.
// Frame-pointer body in the size TU (`optimize("y",off)`): the slot-33 call takes a const reference to an
// empty tag object, which VC homes in the high byte of the argument slot (`lea edx,[ebp+0Bh]`) because
// the tag's user constructor writes nothing.
#include "engine/CItem.h"

struct CItem_Methods : CItem {
    bool IsSameItemTypeAsDefIndex(long defIndex);   // 0x005BC450
};
// Retail std::vector<CItem> (12 bytes). size() is the signed element difference (`sub ; cdq ; idiv 14h`)
// converted to the unsigned size_type; elements are reached through `first`.
struct CItemVector {
    CItem_Methods* first;                        // +0x00
    CItem_Methods* last;                         // +0x04
    CItem_Methods* end_of_storage;               // +0x08
    unsigned long size() const { return (unsigned long)(last - first); }
    bool empty() const { return first == last; }
    CItem_Methods& operator[](unsigned long n) { return first[n]; }
};
// std::find_if over the item range with std::bind2nd(std::mem_fun_ref(&CItem::IsSameItemTypeAsDefIndex), type),
// the VC6-era Dinkumware front end inlined: mem_fun1_ref_t holds the member pointer, binder2nd copies the
// operation and the bound value (8-byte object with user constructors but no copy constructor, so it is
// pushed dword by dword to the __fastcall algorithm instantiation).
struct SItemMemFun {
    bool (CItem_Methods::*m_pfn)(long);
    explicit SItemMemFun(bool (CItem_Methods::*pfn)(long)) : m_pfn(pfn) { }
};
inline SItemMemFun ItemMemFunRef(bool (CItem_Methods::*pfn)(long)) { return SItemMemFun(pfn); }

struct SItemTypeBinder {
    SItemMemFun m_op;                            // +0x00 &CItem::IsSameItemTypeAsDefIndex (0x005BC450)
    long        m_value;                         // +0x04
    SItemTypeBinder(const SItemMemFun& op, const long& value) : m_op(op), m_value(value) { }
};
inline SItemTypeBinder BindItemType(const SItemMemFun& op, const long& value) { return SItemTypeBinder(op, value); }
CItem_Methods* __fastcall FindItemOfType(CItem_Methods* first, CItem_Methods* last, SItemTypeBinder pred);   // 0x005C13E3

struct SSelectTag { SSelectTag() { } };          // empty tag passed by const reference

struct CTCInventoryBase_Calls {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void OnItemOfTypeSelected(long itemType, const SSelectTag& tag);   // slot 33 (+0x84)
};
struct CInventoryCategoryDef;

struct CCategory_Retail {
    unsigned char           PAssociatedThing[0x8];   // +0x00 CIntelligentPointer<CThing>
    CItemVector             ItemsInCategory;         // +0x08
    CInventoryCategoryDef*  PDef_Object;             // +0x14 CDefPointer<const CInventoryCategoryDef>::Object
    CTCInventoryBase_Calls* PInventory;              // +0x18
    long                    GlobalDefIndex;          // +0x1C
    long                    SelectedItemIndex;       // +0x20
    long                    HighlightedItemIndex;    // +0x24
    bool                    Selected;                // +0x28
    bool                    DisplayItems;            // +0x29
    void SetSelectedItemIndex(long index);        // 0x005BC479
    void SelectItemOfType(long itemType);
};

void CCategory_Retail::SelectItemOfType(long itemType)
{
    CItem_Methods* found = FindItemOfType(ItemsInCategory.first, ItemsInCategory.last,
                                          BindItemType(ItemMemFunRef(&CItem_Methods::IsSameItemTypeAsDefIndex), itemType));
    if (found != ItemsInCategory.last) {
        SetSelectedItemIndex((long)(found - ItemsInCategory.first));
        PInventory->OnItemOfTypeSelected(itemType, SSelectTag());
    }
}