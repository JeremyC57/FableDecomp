#pragma optimize("s", on)
struct CBuyHouseDef
{
    void* vftable;
};
extern void* const g_CBuyHouseDef_vftable;
extern void __fastcall CBuyHouseDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CBuyHouseDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CBuyHouseDef_vftable;
    CBuyHouseDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}