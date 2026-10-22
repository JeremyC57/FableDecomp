#pragma optimize("s", on)
struct CAIStateGroup_HoverHornetQueenTakeDamage
{
    void* vftable;
};
extern void* const g_CAIStateGroup_HoverHornetQueenTakeDamage_vftable;
extern void __fastcall CAIStateGroup_HoverHornetQueenTakeDamage_Destruct(void* stateGroup);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CAIStateGroup_HoverHornetQueenTakeDamage* stateGroup, int, unsigned int flags)
{
    stateGroup->vftable = (void*)&g_CAIStateGroup_HoverHornetQueenTakeDamage_vftable;
    CAIStateGroup_HoverHornetQueenTakeDamage_Destruct(stateGroup);
    if (flags & 1)
        operator delete(stateGroup);
    return stateGroup;
}