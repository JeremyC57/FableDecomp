#pragma optimize("s", on)
extern void __fastcall CAIStateGroup_LightWorkCandle_Destruct(void* stateGroup);
void* __fastcall CAIStateGroup_LightWorkCandle_ScalarDeletingDestructor(void* stateGroup, int, unsigned int flags)
{
    CAIStateGroup_LightWorkCandle_Destruct(stateGroup);
    if (flags & 1)
        operator delete(stateGroup);
    return stateGroup;
}