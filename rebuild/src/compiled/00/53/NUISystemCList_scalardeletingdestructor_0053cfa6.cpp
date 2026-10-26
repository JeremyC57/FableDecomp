#pragma optimize("s", on)
extern void __fastcall CList_Destruct(void* list);
void* __fastcall CList_ScalarDeletingDestructor(void* list, int, unsigned int flags)
{
    CList_Destruct(list);
    if (flags & 1)
        operator delete(list);
    return list;
}