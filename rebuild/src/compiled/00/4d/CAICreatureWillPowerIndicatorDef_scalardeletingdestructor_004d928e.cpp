#pragma optimize("s", on)
struct CAICreatureWillPowerIndicatorDef
{
    void* vftable;
};
extern void* const g_CAICreatureWillPowerIndicatorDef_vftable;
extern void __fastcall CAICreatureWillPowerIndicatorDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CAICreatureWillPowerIndicatorDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CAICreatureWillPowerIndicatorDef_vftable;
    CAICreatureWillPowerIndicatorDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}