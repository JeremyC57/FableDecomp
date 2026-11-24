// Leaf: ?GetSizeofClass@CCarrySlotDef@@UAEKXZ
#define FABLE_RETAIL_CCARRYSLOTDEF_RTTI_DECLS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "engine/CCarrySlotDef.h"
#pragma optimize("s",on)
typedef unsigned long ulong;

unsigned long CCarrySlotDef::GetSizeofClass()
{
    return 0x38;
}
// Flatten route (flatten_pilot v4, NF324): CCarrySlotDef_GetSizeofClass -> CCarrySlotDef::GetSizeofClass, the RTTI-placed PDB member; class model: engine/CCarrySlotDef.h opt-in FABLE_RETAIL_CCARRYSLOTDEF_RTTI_DECLS (preamble of CTCCarrying_PutThingInCarrySlot_006a1f80.cpp).
