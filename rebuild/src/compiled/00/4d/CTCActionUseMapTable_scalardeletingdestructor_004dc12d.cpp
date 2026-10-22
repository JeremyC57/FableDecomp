#pragma optimize("s", on)
extern void __fastcall CTCActionUseMapTable_Destruct(void* table);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCActionUseMapTable_ScalarDeletingDestructor(void* table, int, unsigned int flags)
{
    CTCActionUseMapTable_Destruct(table);
    if (flags & 1)
        OperatorDelete(table);
    return table;
}