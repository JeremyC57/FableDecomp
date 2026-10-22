#pragma optimize("s", on)
struct COpinionOfHeroDef
{
    void* vftable;
};
extern void* const g_COpinionOfHeroDef_vftable;
extern void __fastcall COpinionOfHeroDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(COpinionOfHeroDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_COpinionOfHeroDef_vftable;
    COpinionOfHeroDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}