#pragma optimize("s", on)
struct CBedDef
{
    void* vftable;
};
extern void* const g_CBedDef_vftable;
extern void __fastcall CBedDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CBedDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CBedDef_vftable;
    CBedDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}