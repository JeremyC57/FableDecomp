#pragma optimize("s", on)
extern void __fastcall CTCActionUsePutInInventory_Destruct(void* inventory);
void* __fastcall CTCActionUsePutInInventory_ScalarDeletingDestructor(void* inventory, int, unsigned int flags)
{
    CTCActionUsePutInInventory_Destruct(inventory);
    if (flags & 1)
        operator delete(inventory);
    return inventory;
}