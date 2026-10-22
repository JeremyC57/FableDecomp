#pragma optimize("s", on)
extern void __fastcall CTCCreatureItemLevitationHero_Destruct(void* hero);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCCreatureItemLevitationHero_ScalarDeletingDestructor(void* hero, int, unsigned int flags)
{
    CTCCreatureItemLevitationHero_Destruct(hero);
    if (flags & 1)
        OperatorDelete(hero);
    return hero;
}