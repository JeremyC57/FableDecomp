#pragma optimize("s", on)
extern void __fastcall CTCActionUseMapTable_Destruct(void* table);
void* __fastcall CTCActionUseMapTable_ScalarDeletingDestructor(void* table, int, unsigned int flags)
{
    CTCActionUseMapTable_Destruct(table);
    if (flags & 1)
        operator delete(table);
    return table;
}