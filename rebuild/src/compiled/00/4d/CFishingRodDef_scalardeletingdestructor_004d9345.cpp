#pragma optimize("s", on)
struct CFishingRodDef
{
    void* vftable;
};
extern void* const g_CFishingRodDef_vftable;
extern void __fastcall CFishingRodDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CFishingRodDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CFishingRodDef_vftable;
    CFishingRodDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}