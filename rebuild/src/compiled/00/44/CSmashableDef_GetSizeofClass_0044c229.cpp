// Leaf: ?GetSizeofClass@CSmashableDef@@UAEKXZ
#define FABLE_RETAIL_CSMASHABLEDEF_VIRTUAL_MODEL
#define FABLE_RETAIL_DEF_CLASS_BASE
#pragma optimize("s",on)

#include "engine/CSmashableDef.h"  // retyped onto the PDB layout; byte parity re-verified
unsigned long CSmashableDef::GetSizeofClass()
{
    return 0x30;
}
// Flatten route (flatten_pilot v5, NF330): CSmashableDef_GetSizeofClass -> CSmashableDef::GetSizeofClass, the RTTI-placed PDB member; class model: engine/CSmashableDef.h opt-in FABLE_RETAIL_CSMASHABLEDEF_VIRTUAL_MODEL, FABLE_RETAIL_DEF_CLASS_BASE.
