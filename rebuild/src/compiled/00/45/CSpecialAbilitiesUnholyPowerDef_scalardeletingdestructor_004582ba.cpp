#pragma optimize("s",on)
class CSpecialAbilitiesUnholyPowerDef;
extern "C" void __fastcall CSpecialAbilitiesUnholyPowerDef_dtor(CSpecialAbilitiesUnholyPowerDef* def);

void* __fastcall CSpecialAbilitiesUnholyPowerDef_scalar_deleting_destructor(CSpecialAbilitiesUnholyPowerDef* def, int edx_unused, unsigned int flags)
{
    (void)edx_unused;
    CSpecialAbilitiesUnholyPowerDef_dtor(def);
    if (flags & 1)
        operator delete((void*)def);
    return (void*)def;
}