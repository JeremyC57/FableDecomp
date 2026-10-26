#pragma optimize("s", on)
extern void __fastcall CAIStateGroup_MinionSitAtHome_Destruct(void* stateGroup);
void* __fastcall CAIStateGroup_MinionSitAtHome_ScalarDeletingDestructor(void* stateGroup, int, unsigned int flags)
{
    CAIStateGroup_MinionSitAtHome_Destruct(stateGroup);
    if (flags & 1)
        operator delete(stateGroup);
    return stateGroup;
}