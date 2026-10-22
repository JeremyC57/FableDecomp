#pragma optimize("s",on)
struct CSpecialAbilitiesElectrocutedEffectDef { void* vftable; };
extern "C" void __fastcall CSpecialAbilitiesElectrocutedEffectDef_dtor(CSpecialAbilitiesElectrocutedEffectDef* def);
extern "C" void __cdecl eng_operator_delete(void* p);

void* __fastcall CSpecialAbilitiesElectrocutedEffectDef_scalar_deleting_destructor(CSpecialAbilitiesElectrocutedEffectDef* def, int edx, unsigned int flags)
{
    (void)edx;
    CSpecialAbilitiesElectrocutedEffectDef_dtor(def);
    if (flags & 1)
        eng_operator_delete(def);
    return def;
}