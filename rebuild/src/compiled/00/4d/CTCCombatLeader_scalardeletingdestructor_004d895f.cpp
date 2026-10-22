#pragma optimize("s", on)
extern void __fastcall CTCCombatLeader_Destruct(void* leader);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCCombatLeader_ScalarDeletingDestructor(void* leader, int, unsigned int flags)
{
    CTCCombatLeader_Destruct(leader);
    if (flags & 1)
        OperatorDelete(leader);
    return leader;
}