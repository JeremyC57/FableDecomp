#pragma optimize("s", on)
extern void __fastcall CTCInventoryBase_Destruct(void* inventory);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCInventoryBase_ScalarDeletingDestructor(void* inventory, int, unsigned int flags)
{
    CTCInventoryBase_Destruct(inventory);
    if (flags & 1)
        OperatorDelete(inventory);
    return inventory;
}