#pragma optimize("s", on)
extern void __fastcall CTCInventory_Destruct(void* inventory);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCInventory_ScalarDeletingDestructor(void* inventory, int, unsigned int flags)
{
    CTCInventory_Destruct(inventory);
    if (flags & 1)
        OperatorDelete(inventory);
    return inventory;
}