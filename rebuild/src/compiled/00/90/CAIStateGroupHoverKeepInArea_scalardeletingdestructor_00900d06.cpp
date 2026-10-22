#pragma optimize("s", on)
struct CAIStateGroup_HoverKeepInArea
{
    void* vftable;
};
extern void* const g_CAIStateGroup_HoverKeepInArea_vftable;
extern void __fastcall CAIStateGroup_HoverKeepInArea_Destruct(void* stateGroup);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CAIStateGroup_HoverKeepInArea* stateGroup, int, unsigned int flags)
{
    stateGroup->vftable = (void*)&g_CAIStateGroup_HoverKeepInArea_vftable;
    CAIStateGroup_HoverKeepInArea_Destruct(stateGroup);
    if (flags & 1)
        operator delete(stateGroup);
    return stateGroup;
}