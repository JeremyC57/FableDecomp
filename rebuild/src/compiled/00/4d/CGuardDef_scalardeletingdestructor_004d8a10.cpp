#pragma optimize("s", on)
struct CGuardDef
{
    void* vftable;
};
extern void* const g_CGuardDef_vftable;
extern void __fastcall CGuardDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CGuardDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CGuardDef_vftable;
    CGuardDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}