#pragma optimize("s", on)
class CShoveHaPennyRegionDef
{ public:
    void* vftable;
};
extern void* const g_CShoveHaPennyRegionDef_vftable;
extern void __fastcall CShoveHaPennyRegionDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CShoveHaPennyRegionDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CShoveHaPennyRegionDef_vftable;
    CShoveHaPennyRegionDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}