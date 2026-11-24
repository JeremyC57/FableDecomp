// CWeaponDef::GetSizeofClass @ 0x004e26b6
// Leaf: ?GetSizeofClass@CWeaponDef@@UAEKXZ
// Retail: mov eax, 0xe4 ; ret  (returns sizeof-of-class constant)

#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CWeaponDef.h"
unsigned long CWeaponDef::GetSizeofClass()
{
    return 0xe4;
}
// Flatten route (flatten_pilot v4, NF324): CWeaponDef_GetSizeofClass -> CWeaponDef::GetSizeofClass, the RTTI-placed PDB member; class model: engine/CWeaponDef.h under FABLE_RETAIL_DEF_CLASS_BASE.
