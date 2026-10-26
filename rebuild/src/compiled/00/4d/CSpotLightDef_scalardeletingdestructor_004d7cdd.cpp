#pragma optimize("s", on)
class CSpotLightDef
{ public:
    void* vftable;
};
extern void* const g_CSpotLightDef_vftable;
extern void __fastcall CSpotLightDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CSpotLightDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CSpotLightDef_vftable;
    CSpotLightDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}