#pragma optimize("s", on)
extern void __fastcall CAIStateGroup_JackOfBladesStage3_Destruct(void* stateGroup);
void* __fastcall CAIStateGroup_JackOfBladesStage3_ScalarDeletingDestructor(void* stateGroup, int, unsigned int flags)
{
    CAIStateGroup_JackOfBladesStage3_Destruct(stateGroup);
    if (flags & 1)
        operator delete(stateGroup);
    return stateGroup;
}