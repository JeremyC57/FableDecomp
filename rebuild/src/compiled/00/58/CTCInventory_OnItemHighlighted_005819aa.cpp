#pragma optimize("s", on)
// CTCInventory::OnItemHighlighted(long item) -- resets the item preview, runs the base notification, then refreshes the quick-assign / usable flags and the owned count for the newly highlighted item
// Leaf: OnItemHighlighted
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005819AA, length 51 bytes.
// Callees (all NInventory::CTCInventoryBase, __fastcall, one long): ResetInventoryAnim 0x005BCD2D,
//          OnItemHighlighted 0x005BC4C0 (the base's empty virtual, called qualified),
//          SetCanItemBeQuickAssigned 0x005BC505 (writes +0x34), SetCanItemBeUsed 0x005BC4DF
//          (writes +0x35; manifest calls it SetCanItemBeQuickAssigned too), SetNumberOfSelectedItemOwned 0x005BE284.
// Vtable slot 36 of CTCInventory (0x01250DE4); the base vtable 0x01253DBC holds 0x005BC4C0 there.
// Ego_r PDB: ?OnItemHighlighted@CTCInventory@@UAEXJ@Z.
// Shape: the argument is loaded once into edi and pushed before every call.
struct CTCInventoryBase {
    void ResetInventoryAnim(long item);              // 0x005BCD2D
    void OnItemHighlighted(long item);               // 0x005BC4C0
    void SetCanItemBeQuickAssigned(long item);       // 0x005BC505
    void SetCanItemBeUsed(long item);                // 0x005BC4DF
    void SetNumberOfSelectedItemOwned(long item);    // 0x005BE284
};

struct CTCInventory : CTCInventoryBase {
    void OnItemHighlighted(long item);
};

void CTCInventory::OnItemHighlighted(long item)
{
    ResetInventoryAnim(item);
    CTCInventoryBase::OnItemHighlighted(item);
    SetCanItemBeQuickAssigned(item);
    SetCanItemBeUsed(item);
    SetNumberOfSelectedItemOwned(item);
}