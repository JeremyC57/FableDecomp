// CTCInventoryBase::GetItemOfTypeInInventory(long itemType) -- when the inventory holds at least one item of `itemType`, creates and returns a thing for it, else null
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BF3F1, length 35 bytes; size TU (pass-through `push dword ptr [esp+8]`).
// Virtual slot 47 of the inventory screens (CTCInventoryClothing vtable 0x0124356C slot 47).
// Callees: CTCInventoryBase::GetNumberOfItemsOfTypeInInventory(long) @ 0x005BDF08, CTCInventoryBase::CreateInventoryObject(long) @ 0x005BE8F3.
// Retail CTCInventoryBase: only the two member calls are used (no data members touched).
#pragma optimize("s", on)
struct CThing;
struct CTCInventoryBase_Retail {
    long    GetNumberOfItemsOfTypeInInventory(long itemType);   // 0x005BDF08
    CThing* CreateInventoryObject(long itemType);               // 0x005BE8F3
    CThing* GetItemOfTypeInInventory(long itemType);
};
CThing* CTCInventoryBase_Retail::GetItemOfTypeInInventory(long itemType)
{
    if (GetNumberOfItemsOfTypeInInventory(itemType) > 0)
        return CreateInventoryObject(itemType);
    return 0;
}