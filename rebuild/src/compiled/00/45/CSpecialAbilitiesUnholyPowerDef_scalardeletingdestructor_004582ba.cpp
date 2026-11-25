#pragma optimize("s",on)
// Naming (NF451, 2026-11-25): callee-only respell of the destructor stand-in call (@CSpecialAbilitiesUnholyPowerDef_dtor@4) to
//   0x004582D6 ??1CSpecialAbilitiesUnholyPowerDef@@UAE@XZ (the landed target row's Leaf; FableWin.pdb, registered as a one-source best guess), a qualified
//   destructor call through engine/CSpecialAbilitiesUnholyPowerDef.h (its add-only destructor-only opt-in, so no STLport and no new
//   .drectve token).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CSPECIALABILITIESUNHOLYPOWERDEF_DTOR_DECL_ONLY
#include "engine/CSpecialAbilitiesUnholyPowerDef.h"

void* __fastcall CSpecialAbilitiesUnholyPowerDef_scalar_deleting_destructor(CSpecialAbilitiesUnholyPowerDef* def, int edx_unused, unsigned int flags)
{
    (void)edx_unused;
    def->CSpecialAbilitiesUnholyPowerDef::~CSpecialAbilitiesUnholyPowerDef();
    if (flags & 1)
        operator delete((void*)def);
    return (void*)def;
}