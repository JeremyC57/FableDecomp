#pragma optimize("s", on)
extern void __fastcall CTCCreatureGroup_Destruct(void* group);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCCreatureGroup_ScalarDeletingDestructor(void* group, int, unsigned int flags)
{
    CTCCreatureGroup_Destruct(group);
    if (flags & 1)
        OperatorDelete(group);
    return group;
}