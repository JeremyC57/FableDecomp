#pragma optimize("s", on)
struct CFishDef
{
    void* vftable;
};
extern void* const g_CFishDef_vftable;
extern void __fastcall CFishDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CFishDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CFishDef_vftable;
    CFishDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}