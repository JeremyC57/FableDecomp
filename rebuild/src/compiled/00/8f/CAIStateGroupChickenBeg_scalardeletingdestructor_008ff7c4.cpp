#pragma optimize("s", on)
extern void __fastcall CAIStateGroup_ChickenBeg_Destruct(void* stateGroup);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CAIStateGroup_ChickenBeg_ScalarDeletingDestructor(void* stateGroup, int, unsigned int flags)
{
    CAIStateGroup_ChickenBeg_Destruct(stateGroup);
    if (flags & 1)
        OperatorDelete(stateGroup);
    return stateGroup;
}