#pragma optimize("s", on)
struct CEnemyDef
{
    void* vftable;
};
extern void* const g_CEnemyDef_vftable;
extern void __fastcall CEnemyDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CEnemyDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CEnemyDef_vftable;
    CEnemyDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}