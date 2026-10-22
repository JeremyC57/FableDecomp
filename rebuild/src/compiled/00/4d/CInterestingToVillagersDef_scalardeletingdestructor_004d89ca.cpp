#pragma optimize("s", on)
struct CInterestingToVillagersDef
{
    void* vftable;
};
extern void* const g_CInterestingToVillagersDef_vftable;
extern void __fastcall CInterestingToVillagersDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CInterestingToVillagersDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CInterestingToVillagersDef_vftable;
    CInterestingToVillagersDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}