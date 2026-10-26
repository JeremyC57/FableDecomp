#pragma optimize("s", on)
extern void __fastcall CTCInventoryBase_Destruct(void* inventory);
void* __fastcall CTCInventoryBase_ScalarDeletingDestructor(void* inventory, int, unsigned int flags)
{
    CTCInventoryBase_Destruct(inventory);
    if (flags & 1)
        operator delete(inventory);
    return inventory;
}