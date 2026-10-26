#pragma optimize("s", on)
extern void __fastcall CAIStateGroup_ApprenticeHero_Destruct(void* stateGroup);
void* __fastcall CAIStateGroup_ApprenticeHero_ScalarDeletingDestructor(void* stateGroup, int, unsigned int flags)
{
    CAIStateGroup_ApprenticeHero_Destruct(stateGroup);
    if (flags & 1)
        operator delete(stateGroup);
    return stateGroup;
}