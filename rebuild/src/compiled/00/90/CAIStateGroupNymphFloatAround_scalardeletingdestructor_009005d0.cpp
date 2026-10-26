#pragma optimize("s", on)
extern void __fastcall CAIStateGroup_NymphFloatAround_Destruct(void* stateGroup);
void* __fastcall CAIStateGroup_NymphFloatAround_ScalarDeletingDestructor(void* stateGroup, int, unsigned int flags)
{
    CAIStateGroup_NymphFloatAround_Destruct(stateGroup);
    if (flags & 1)
        operator delete(stateGroup);
    return stateGroup;
}