#pragma optimize("s", on)
extern void __fastcall CTCCreatureItemLevitationHero_Destruct(void* hero);
void* __fastcall CTCCreatureItemLevitationHero_ScalarDeletingDestructor(void* hero, int, unsigned int flags)
{
    CTCCreatureItemLevitationHero_Destruct(hero);
    if (flags & 1)
        operator delete(hero);
    return hero;
}