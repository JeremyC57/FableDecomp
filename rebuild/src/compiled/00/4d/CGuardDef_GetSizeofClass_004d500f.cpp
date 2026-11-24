// Leaf: ?GetSizeofClass@CGuardDef@@UAEKXZ
#define FABLE_RETAIL_CGUARDDEF_VIRTUAL_MODEL
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CGuardDef.h"
#pragma optimize("s",on)
unsigned long CGuardDef::GetSizeofClass()
{
    return 80;
}
// Flatten route (flatten_pilot v5, NF330): CGuardDef_GetSizeofClass -> CGuardDef::GetSizeofClass, the RTTI-placed PDB member; class model: engine/CGuardDef.h opt-in FABLE_RETAIL_CGUARDDEF_VIRTUAL_MODEL, FABLE_RETAIL_DEF_CLASS_BASE.
