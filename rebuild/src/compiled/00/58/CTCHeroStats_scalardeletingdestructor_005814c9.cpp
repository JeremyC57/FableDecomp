#pragma optimize("s", on)
extern void __fastcall CTCHeroStats_Destruct(void* stats);
void* __fastcall CTCHeroStats_ScalarDeletingDestructor(void* stats, int, unsigned int flags)
{
    CTCHeroStats_Destruct(stats);
    if (flags & 1)
        operator delete(stats);
    return stats;
}