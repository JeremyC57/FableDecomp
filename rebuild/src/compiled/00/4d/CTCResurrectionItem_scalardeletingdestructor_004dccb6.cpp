#pragma optimize("s", on)
extern void __fastcall CTCResurrectionItem_Destruct(void* item);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCResurrectionItem_ScalarDeletingDestructor(void* item, int, unsigned int flags)
{
    CTCResurrectionItem_Destruct(item);
    if (flags & 1)
        OperatorDelete(item);
    return item;
}