#pragma optimize("s", on)
extern void __fastcall CTCActionUsePutInInventory_Destruct(void* inventory);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCActionUsePutInInventory_ScalarDeletingDestructor(void* inventory, int, unsigned int flags)
{
    CTCActionUsePutInInventory_Destruct(inventory);
    if (flags & 1)
        OperatorDelete(inventory);
    return inventory;
}