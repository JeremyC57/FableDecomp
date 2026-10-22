#pragma optimize("s", on)
struct CCarriedReadableDef
{
    void* vftable;
};
extern void* const g_CCarriedReadableDef_vftable;
extern void __fastcall CCarriedReadableDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CCarriedReadableDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CCarriedReadableDef_vftable;
    CCarriedReadableDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}