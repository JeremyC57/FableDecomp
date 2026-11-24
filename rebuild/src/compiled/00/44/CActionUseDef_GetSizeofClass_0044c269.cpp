// Leaf: ?GetSizeofClass@CActionUseDef@@UAEKXZ
#define FABLE_RETAIL_CACTIONUSEDEF_VIRTUAL_MODEL
#define FABLE_RETAIL_DEF_CLASS_BASE
#pragma optimize("s",on)
#include "engine/CActionUseDef.h"  // retyped onto the PDB layout; byte parity re-verified
typedef unsigned long ulong;


unsigned long CActionUseDef::GetSizeofClass()
{
    return 0x34;
}
// Flatten route (flatten_pilot v5, NF330): CActionUseDef_GetSizeofClass -> CActionUseDef::GetSizeofClass, the RTTI-placed PDB member; class model: engine/CActionUseDef.h opt-in FABLE_RETAIL_CACTIONUSEDEF_VIRTUAL_MODEL, FABLE_RETAIL_DEF_CLASS_BASE.
