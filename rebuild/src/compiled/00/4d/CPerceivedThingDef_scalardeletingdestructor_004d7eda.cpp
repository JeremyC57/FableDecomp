#pragma optimize("s", on)
struct CPerceivedThingDef
{
    void* vftable;
};
extern void* const g_CPerceivedThingDef_vftable;
extern void __fastcall CPerceivedThingDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CPerceivedThingDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CPerceivedThingDef_vftable;
    CPerceivedThingDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}