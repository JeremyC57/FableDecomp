#pragma optimize("s", on)
struct CKrakenTentacleDef
{
    void* vftable;
};
extern void* const g_CKrakenTentacleDef_vftable;
extern void __fastcall CKrakenTentacleDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CKrakenTentacleDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CKrakenTentacleDef_vftable;
    CKrakenTentacleDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}