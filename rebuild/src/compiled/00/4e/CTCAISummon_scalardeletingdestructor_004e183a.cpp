#pragma optimize("s", on)
extern void __fastcall CTCAISummon_Destruct(void* summon);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCAISummon_ScalarDeletingDestructor(void* summon, int, unsigned int flags)
{
    CTCAISummon_Destruct(summon);
    if (flags & 1)
        OperatorDelete(summon);
    return summon;
}