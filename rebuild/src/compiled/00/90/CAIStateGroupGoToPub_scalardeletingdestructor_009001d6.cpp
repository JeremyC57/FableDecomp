#pragma optimize("s", on)
extern void __fastcall CAIStateGroup_GoToPub_Destruct(void* stateGroup);
void* __fastcall CAIStateGroup_GoToPub_ScalarDeletingDestructor(void* stateGroup, int, unsigned int flags)
{
    CAIStateGroup_GoToPub_Destruct(stateGroup);
    if (flags & 1)
        operator delete(stateGroup);
    return stateGroup;
}