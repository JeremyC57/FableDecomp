#pragma optimize("s", on)
struct CThingDrainLifeShotDef
{
    void* vftable;
};
extern void* const g_CThingDrainLifeShotDef_vftable;
extern void __fastcall CThingDrainLifeShotDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CThingDrainLifeShotDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CThingDrainLifeShotDef_vftable;
    CThingDrainLifeShotDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}