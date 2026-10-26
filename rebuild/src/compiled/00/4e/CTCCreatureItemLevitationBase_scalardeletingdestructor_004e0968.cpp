#pragma optimize("s", on)
extern void __fastcall CTCCreatureItemLevitationBase_Destruct(void* levitation);
void* __fastcall CTCCreatureItemLevitationBase_ScalarDeletingDestructor(void* levitation, int, unsigned int flags)
{
    CTCCreatureItemLevitationBase_Destruct(levitation);
    if (flags & 1)
        operator delete(levitation);
    return levitation;
}