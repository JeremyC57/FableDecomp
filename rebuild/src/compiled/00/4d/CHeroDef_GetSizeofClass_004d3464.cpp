// Leaf: ?GetSizeofClass@CHeroDef@@UAEKXZ
#define FABLE_RETAIL_CHERODEF_VIRTUAL_MODEL
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CHeroDef.h"
#pragma optimize("s",on)
unsigned long CHeroDef::GetSizeofClass()
{
    return 48;
}
// Flatten route (flatten_pilot v5, NF330): CHeroDef_GetSizeofClass -> CHeroDef::GetSizeofClass, the RTTI-placed PDB member; class model: engine/CHeroDef.h opt-in FABLE_RETAIL_CHERODEF_VIRTUAL_MODEL, FABLE_RETAIL_DEF_CLASS_BASE.
