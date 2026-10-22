#pragma optimize("s",on)
struct CSpecialAbilitiesHealLifeDef { void* vftable; };
extern "C" void __fastcall CSpecialAbilitiesHealLifeDef_dtor(CSpecialAbilitiesHealLifeDef* def);
extern "C" void __cdecl fable_operator_delete(void* p);

void* __fastcall CSpecialAbilitiesHealLifeDef_scalar_deleting_destructor(CSpecialAbilitiesHealLifeDef* def, int edx_unused, unsigned int flags)
{
    CSpecialAbilitiesHealLifeDef_dtor(def);
    if (flags & 1)
        fable_operator_delete(def);
    return def;
}