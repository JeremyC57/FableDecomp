#pragma optimize("s",on)
class CSpecialAbilitiesLightningSpellDef { public: void* vftable; };

extern void __fastcall CSpecialAbilitiesLightningSpellDef_dtor(CSpecialAbilitiesLightningSpellDef* def);
void __cdecl operator delete(void* p);

void* __fastcall CSpecialAbilitiesLightningSpellDef_scalar_deleting_destructor(CSpecialAbilitiesLightningSpellDef* def, int edx_pad, unsigned int flags)
{
    CSpecialAbilitiesLightningSpellDef_dtor(def);
    if (flags & 1)
        operator delete((void*)def);
    return def;
}