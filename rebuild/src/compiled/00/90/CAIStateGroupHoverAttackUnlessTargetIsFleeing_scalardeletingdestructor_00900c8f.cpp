#pragma optimize("s", on)
struct CAIStateGroup_HoverAttackUnlessTargetIsFleeing
{
    void* vftable;
};
extern void* const g_CAIStateGroup_HoverAttackUnlessTargetIsFleeing_vftable;
extern void __fastcall CAIStateGroup_HoverAttackUnlessTargetIsFleeing_Destruct(void* stateGroup);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CAIStateGroup_HoverAttackUnlessTargetIsFleeing* stateGroup, int, unsigned int flags)
{
    stateGroup->vftable = (void*)&g_CAIStateGroup_HoverAttackUnlessTargetIsFleeing_vftable;
    CAIStateGroup_HoverAttackUnlessTargetIsFleeing_Destruct(stateGroup);
    if (flags & 1)
        operator delete(stateGroup);
    return stateGroup;
}