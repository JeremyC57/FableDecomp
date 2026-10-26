#pragma optimize("s", on)
extern void __fastcall CTCAISummon_Destruct(void* summon);
void* __fastcall CTCAISummon_ScalarDeletingDestructor(void* summon, int, unsigned int flags)
{
    CTCAISummon_Destruct(summon);
    if (flags & 1)
        operator delete(summon);
    return summon;
}