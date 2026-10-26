#pragma optimize("s", on)
extern void __fastcall CAIStateGroup_MeleeCombatNearby2_Destruct(void* stateGroup);
void* __fastcall CAIStateGroup_MeleeCombatNearby2_ScalarDeletingDestructor(void* stateGroup, int, unsigned int flags)
{
    CAIStateGroup_MeleeCombatNearby2_Destruct(stateGroup);
    if (flags & 1)
        operator delete(stateGroup);
    return stateGroup;
}