#pragma optimize("s",on)
class CSpecialAbilitiesHealLifeDef { public: void* vftable; };
extern "C" void __fastcall CSpecialAbilitiesHealLifeDef_dtor(CSpecialAbilitiesHealLifeDef* def);

void* __fastcall CSpecialAbilitiesHealLifeDef_scalar_deleting_destructor(CSpecialAbilitiesHealLifeDef* def, int edx_unused, unsigned int flags)
{
    CSpecialAbilitiesHealLifeDef_dtor(def);
    if (flags & 1)
        operator delete(def);
    return def;
}