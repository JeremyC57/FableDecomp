#pragma optimize("s", on)
extern void __fastcall CTCInventoryItem_Destruct(void* item);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCInventoryItem_ScalarDeletingDestructor(void* item, int, unsigned int flags)
{
    CTCInventoryItem_Destruct(item);
    if (flags & 1)
        OperatorDelete(item);
    return item;
}