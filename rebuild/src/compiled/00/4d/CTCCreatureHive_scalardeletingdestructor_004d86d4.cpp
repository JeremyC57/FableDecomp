#pragma optimize("s", on)
extern void __fastcall CTCCreatureHive_Destruct(void* hive);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCCreatureHive_ScalarDeletingDestructor(void* hive, int, unsigned int flags)
{
    CTCCreatureHive_Destruct(hive);
    if (flags & 1)
        OperatorDelete(hive);
    return hive;
}