#pragma optimize("s", on)
struct CLightDef
{
    void* vftable;
};
extern void* const g_CLightDef_vftable;
extern void __fastcall CLightDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CLightDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CLightDef_vftable;
    CLightDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}