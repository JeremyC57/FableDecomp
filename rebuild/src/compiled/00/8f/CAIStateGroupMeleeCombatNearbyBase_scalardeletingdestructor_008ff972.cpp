#pragma optimize("s", on)
extern void __fastcall CAIStateGroup_MeleeCombatNearbyBase_Destruct(void* stateGroup);
void* __fastcall CAIStateGroup_MeleeCombatNearbyBase_ScalarDeletingDestructor(void* stateGroup, int, unsigned int flags)
{
    CAIStateGroup_MeleeCombatNearbyBase_Destruct(stateGroup);
    if (flags & 1)
        operator delete(stateGroup);
    return stateGroup;
}