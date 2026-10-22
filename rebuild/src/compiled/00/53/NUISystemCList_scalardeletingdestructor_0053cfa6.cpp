#pragma optimize("s", on)
extern void __fastcall CList_Destruct(void* list);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CList_ScalarDeletingDestructor(void* list, int, unsigned int flags)
{
    CList_Destruct(list);
    if (flags & 1)
        OperatorDelete(list);
    return list;
}