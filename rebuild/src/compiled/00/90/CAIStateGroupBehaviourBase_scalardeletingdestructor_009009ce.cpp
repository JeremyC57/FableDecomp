#pragma optimize("s", on)
extern void __fastcall CAIStateGroupBehaviourBase_Destruct(void* stateGroup);
void* __fastcall CAIStateGroupBehaviourBase_ScalarDeletingDestructor(void* stateGroup, int, unsigned int flags)
{
    CAIStateGroupBehaviourBase_Destruct(stateGroup);
    if (flags & 1)
        operator delete(stateGroup);
    return stateGroup;
}