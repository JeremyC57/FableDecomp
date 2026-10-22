#pragma optimize("s", on)
struct CBoastingPodiumDef
{
    void* vftable;
};
extern void* const g_CBoastingPodiumDef_vftable;
extern void __fastcall CBoastingPodiumDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CBoastingPodiumDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CBoastingPodiumDef_vftable;
    CBoastingPodiumDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}