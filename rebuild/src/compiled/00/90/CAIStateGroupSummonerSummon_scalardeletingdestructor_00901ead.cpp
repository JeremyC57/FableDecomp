#pragma optimize("s", on)
extern void __fastcall DestructMember(void* member);
extern void __fastcall CAIStateGroup_SummonerSummon_Destruct(void* stateGroup);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CAIStateGroup_SummonerSummon_CompositeScalarDeletingDestructor(void* stateGroup, int, unsigned int flags)
{
    DestructMember(static_cast<unsigned char*>(stateGroup) + 20);
    CAIStateGroup_SummonerSummon_Destruct(stateGroup);
    if (flags & 1)
        OperatorDelete(stateGroup);
    return stateGroup;
}