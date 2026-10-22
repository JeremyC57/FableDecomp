#pragma optimize("s", on)
struct CTavernDef
{
    void* vftable;
};
extern void* const g_CTavernDef_vftable;
extern void __fastcall CTavernDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CTavernDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CTavernDef_vftable;
    CTavernDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}