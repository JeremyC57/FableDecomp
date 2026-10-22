#pragma optimize("s", on)
struct CNymphDef
{
    void* vftable;
};
extern void* const g_CNymphDef_vftable;
extern void __fastcall CNymphDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CNymphDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CNymphDef_vftable;
    CNymphDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}