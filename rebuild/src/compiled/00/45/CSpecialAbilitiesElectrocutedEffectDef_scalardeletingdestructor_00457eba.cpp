#pragma optimize("s",on)
class CSpecialAbilitiesElectrocutedEffectDef { public: void* vftable; };
extern "C" void __fastcall CSpecialAbilitiesElectrocutedEffectDef_dtor(CSpecialAbilitiesElectrocutedEffectDef* def);

void* __fastcall CSpecialAbilitiesElectrocutedEffectDef_scalar_deleting_destructor(CSpecialAbilitiesElectrocutedEffectDef* def, int edx, unsigned int flags)
{
    (void)edx;
    CSpecialAbilitiesElectrocutedEffectDef_dtor(def);
    if (flags & 1)
        operator delete(def);
    return def;
}