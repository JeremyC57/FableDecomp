#pragma optimize("s", on)
extern void __fastcall CTCCreatureGroup_Destruct(void* group);
void* __fastcall CTCCreatureGroup_ScalarDeletingDestructor(void* group, int, unsigned int flags)
{
    CTCCreatureGroup_Destruct(group);
    if (flags & 1)
        operator delete(group);
    return group;
}