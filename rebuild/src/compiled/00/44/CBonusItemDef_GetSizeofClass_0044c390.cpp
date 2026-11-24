// Leaf: ?GetSizeofClass@CBonusItemDef@@UAEKXZ
#define FABLE_RETAIL_CBONUSITEMDEF_VIRTUAL_MODEL
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CBonusItemDef.h"
#pragma optimize("s",on)
typedef unsigned long ulong;

unsigned long CBonusItemDef::GetSizeofClass()
{
    return 0x74;
}
// Flatten route (flatten_pilot v5, NF330): CBonusItemDef_GetSizeofClass -> CBonusItemDef::GetSizeofClass, the RTTI-placed PDB member; class model: engine/CBonusItemDef.h opt-in FABLE_RETAIL_CBONUSITEMDEF_VIRTUAL_MODEL, FABLE_RETAIL_DEF_CLASS_BASE.
