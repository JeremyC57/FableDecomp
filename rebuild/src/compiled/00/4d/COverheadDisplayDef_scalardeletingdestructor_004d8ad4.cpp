#pragma optimize("s", on)
struct COverheadDisplayDef
{
    void* vftable;
};
extern void* const g_COverheadDisplayDef_vftable;
extern void __fastcall COverheadDisplayDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(COverheadDisplayDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_COverheadDisplayDef_vftable;
    COverheadDisplayDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}