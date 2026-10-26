#pragma optimize("s", on)
extern void __fastcall CAIStateGroup_CreatureFindCover_Destruct(void* stateGroup);
void* __fastcall CAIStateGroup_CreatureFindCover_ScalarDeletingDestructor(void* stateGroup, int, unsigned int flags)
{
    CAIStateGroup_CreatureFindCover_Destruct(stateGroup);
    if (flags & 1)
        operator delete(stateGroup);
    return stateGroup;
}