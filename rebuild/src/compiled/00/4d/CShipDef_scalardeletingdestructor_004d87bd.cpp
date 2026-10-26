#pragma optimize("s", on)
class CShipDef
{ public:
    void* vftable;
};
extern void* const g_CShipDef_vftable;
extern void __fastcall CShipDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CShipDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CShipDef_vftable;
    CShipDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}