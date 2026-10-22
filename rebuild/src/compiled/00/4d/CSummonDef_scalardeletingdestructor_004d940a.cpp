#pragma optimize("s", on)
struct CSummonDef
{
    void* vftable;
};
extern void* const g_CSummonDef_vftable;
extern void __fastcall CSummonDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CSummonDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CSummonDef_vftable;
    CSummonDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}