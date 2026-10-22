#pragma optimize("s", on)
struct CAIStateGroup_HoverWander
{
    void* vftable;
};
extern void* const g_CAIStateGroup_HoverWander_vftable;
extern void __fastcall CAIStateGroup_HoverWander_Destruct(void* stateGroup);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CAIStateGroup_HoverWander* stateGroup, int, unsigned int flags)
{
    stateGroup->vftable = (void*)&g_CAIStateGroup_HoverWander_vftable;
    CAIStateGroup_HoverWander_Destruct(stateGroup);
    if (flags & 1)
        operator delete(stateGroup);
    return stateGroup;
}