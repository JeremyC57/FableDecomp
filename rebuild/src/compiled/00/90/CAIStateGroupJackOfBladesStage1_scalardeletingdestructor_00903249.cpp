#pragma optimize("s", on)
extern void __fastcall CAIStateGroup_JackOfBladesStage1_Destruct(void* stateGroup);
void* __fastcall CAIStateGroup_JackOfBladesStage1_ScalarDeletingDestructor(void* stateGroup, int, unsigned int flags)
{
    CAIStateGroup_JackOfBladesStage1_Destruct(stateGroup);
    if (flags & 1)
        operator delete(stateGroup);
    return stateGroup;
}