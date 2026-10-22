#pragma optimize("s", on)
struct CTeleporterDef
{
    void* vftable;
};
extern void* const g_CTeleporterDef_vftable;
extern void __fastcall CTeleporterDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CTeleporterDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CTeleporterDef_vftable;
    CTeleporterDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}