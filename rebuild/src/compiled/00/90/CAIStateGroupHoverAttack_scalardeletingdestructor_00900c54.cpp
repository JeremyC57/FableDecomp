#pragma optimize("s", on)
struct CAIStateGroup_HoverAttack
{
    void* vftable;
};
extern void* const g_CAIStateGroup_HoverAttack_vftable;
extern void __fastcall CAIStateGroup_HoverAttack_Destruct(void* stateGroup);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CAIStateGroup_HoverAttack* stateGroup, int, unsigned int flags)
{
    stateGroup->vftable = (void*)&g_CAIStateGroup_HoverAttack_vftable;
    CAIStateGroup_HoverAttack_Destruct(stateGroup);
    if (flags & 1)
        operator delete(stateGroup);
    return stateGroup;
}