#pragma optimize("s", on)
struct CDrunkennessDef
{
    void* vftable;
};
extern void* const g_CDrunkennessDef_vftable;
extern void __fastcall CDrunkennessDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CDrunkennessDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CDrunkennessDef_vftable;
    CDrunkennessDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}