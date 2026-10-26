#pragma optimize("s", on)
class CCreatureNavigationDef
{ public:
    void* vftable;
};
extern void* const g_CCreatureNavigationDef_vftable;
extern void __fastcall CCreatureNavigationDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CCreatureNavigationDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CCreatureNavigationDef_vftable;
    CCreatureNavigationDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}