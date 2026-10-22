#pragma optimize("s", on)
extern void __fastcall CTCCreatureItemLevitationBase_Destruct(void* levitation);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCCreatureItemLevitationBase_ScalarDeletingDestructor(void* levitation, int, unsigned int flags)
{
    CTCCreatureItemLevitationBase_Destruct(levitation);
    if (flags & 1)
        OperatorDelete(levitation);
    return levitation;
}