#pragma optimize("s", on)
struct CResurrectionItemDef
{
    void* vftable;
};
extern void* const g_CResurrectionItemDef_vftable;
extern void __fastcall CResurrectionItemDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CResurrectionItemDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CResurrectionItemDef_vftable;
    CResurrectionItemDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}