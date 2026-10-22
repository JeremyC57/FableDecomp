#pragma optimize("s", on)
struct CBettingDef
{
    void* vftable;
};
extern void* const g_CBettingDef_vftable;
extern void __fastcall CBettingDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CBettingDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CBettingDef_vftable;
    CBettingDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}