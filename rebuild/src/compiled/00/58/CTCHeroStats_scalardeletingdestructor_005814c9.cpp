#pragma optimize("s", on)
extern void __fastcall CTCHeroStats_Destruct(void* stats);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCHeroStats_ScalarDeletingDestructor(void* stats, int, unsigned int flags)
{
    CTCHeroStats_Destruct(stats);
    if (flags & 1)
        OperatorDelete(stats);
    return stats;
}