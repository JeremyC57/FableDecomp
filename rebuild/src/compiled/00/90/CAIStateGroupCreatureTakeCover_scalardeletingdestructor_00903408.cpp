#pragma optimize("s", on)
extern void __fastcall CAIStateGroup_CreatureTakeCover_Destruct(void* stateGroup);
void* __fastcall CAIStateGroup_CreatureTakeCover_ScalarDeletingDestructor(void* stateGroup, int, unsigned int flags)
{
    CAIStateGroup_CreatureTakeCover_Destruct(stateGroup);
    if (flags & 1)
        operator delete(stateGroup);
    return stateGroup;
}