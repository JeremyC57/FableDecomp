#pragma optimize("s", on)
extern void __fastcall CTCStockItem_Destruct(void* item);
void* __fastcall CTCStockItem_ScalarDeletingDestructor(void* item, int, unsigned int flags)
{
    CTCStockItem_Destruct(item);
    if (flags & 1)
        operator delete(item);
    return item;
}