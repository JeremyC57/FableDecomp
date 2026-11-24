// Leaf: ?GetSizeofClass@CShipDef@@UAEKXZ
#define FABLE_RETAIL_CSHIPDEF_VIRTUAL_MODEL
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CShipDef.h"
#pragma optimize("s",on)
unsigned long CShipDef::GetSizeofClass()
{
    return 68;
}
// Flatten route (flatten_pilot v5, NF330): CShipDef_GetSizeofClass -> CShipDef::GetSizeofClass, the RTTI-placed PDB member; class model: engine/CShipDef.h opt-in FABLE_RETAIL_CSHIPDEF_VIRTUAL_MODEL, FABLE_RETAIL_DEF_CLASS_BASE.
