// CInventoryCategoryDef::Transfer(CPersistContext& context) -- persist the def's fields one by one (Inventory, NumberOfSlots, DrawItemSlots, SelectEmptySlots, WrapHighlightCursor, CategoryName, AllowItemsToFillMoreThanOneSlot, CategoryIdentifier, AddCategoryOnCreate)
// Leaf: ?Transfer@CInventoryCategoryDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00455588, 112 bytes (size-optimised def TU, odd entry), thiscall, `ret 4`.
// Identity: CInventoryCategoryDef::Transfer(CPersistContext&) -- proven; evidence: the catalogue row is this virtual (manifest
//   ?Transfer@CInventoryCategoryDef@@UAEXAAVCPersistContext@@@Z, Ego_r.pdb name set); the nine transferred offsets and their callee
//   overloads match Ego_r.pdb CInventoryCategoryDef's members and types exactly (retail offset = Ego_r offset - 4: the TLC def base is 0x3c bytes). Family G057 (seven def Transfers of nine fields).
// Callees: CPersistContext::Transfer(unsigned long&) 0x00431020, Transfer(long&) 0x00431102, Transfer(float&) 0x00431061,
//   Transfer(bool&) 0x0043314A (each bound under that name by landed defs); member-template instances Transfer<EInventoryCategoryIdentifier> 0x004555F8.
// Shape: context in edi and this in esi for the whole body; the last field's address is `add esi, N` in place.
#pragma optimize("s", on)
enum EInventoryCategoryIdentifier { EInventoryCategoryIdentifier_Force32 = 0x7fffffff };
class CPersistContext {
public:
    void Transfer(unsigned long& value);                   // 0x00431020
    void Transfer(long& value);                            // 0x00431102
    void Transfer(float& value);                           // 0x00431061
    void Transfer(bool& value);                            // 0x0043314A
    template <class T> void Transfer(T& value);            // enum / container instances in this TU
};
class CInventoryCategoryDef {                                              // retail fields from +0x3c
public:
    virtual void Transfer(CPersistContext& context);
    unsigned char _base[0x38];                    // +0x04 def base body
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
void CInventoryCategoryDef::Transfer(CPersistContext& context)
{
    context.Transfer(Inventory);
    context.Transfer(NumberOfSlots);
    context.Transfer(DrawItemSlots);
    context.Transfer(SelectEmptySlots);
    context.Transfer(WrapHighlightCursor);
    context.Transfer(CategoryName);
    context.Transfer(AllowItemsToFillMoreThanOneSlot);
    context.Transfer(CategoryIdentifier);
    context.Transfer(AddCategoryOnCreate);
}