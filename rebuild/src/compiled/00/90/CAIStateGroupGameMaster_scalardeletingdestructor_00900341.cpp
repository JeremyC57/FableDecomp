#pragma optimize("s", on)
extern void __fastcall CAIStateGroup_GameMaster_Destruct(void* stateGroup);
void* __fastcall CAIStateGroup_GameMaster_ScalarDeletingDestructor(void* stateGroup, int, unsigned int flags)
{
    CAIStateGroup_GameMaster_Destruct(stateGroup);
    if (flags & 1)
        operator delete(stateGroup);
    return stateGroup;
}