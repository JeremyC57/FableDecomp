#pragma optimize("s", on)
struct CWifeDef
{
    void* vftable;
};
extern void* const g_CWifeDef_vftable;
extern void __fastcall CWifeDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CWifeDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CWifeDef_vftable;
    CWifeDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}