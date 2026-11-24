// Leaf: ?GetSizeofClass@CHairCardDef@@UAEKXZ
#define FABLE_RETAIL_CHAIRCARDDEF_RTTI_DECLS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CHAIRCARDDEF
#include "fable_string.h"
#include "engine/CHairCardDef.h"
#pragma optimize("s",on)
typedef unsigned long ulong;

unsigned long CHairCardDef::GetSizeofClass()
{
    return 0x2c;
}
// Flatten route (flatten_pilot v4, NF324): CHairCardDef_GetSizeofClass -> CHairCardDef::GetSizeofClass, the RTTI-placed PDB member; class model: engine/CHairCardDef.h opt-in FABLE_RETAIL_CHAIRCARDDEF, FABLE_RETAIL_CHAIRCARDDEF_RTTI_DECLS (preamble of CTCInventoryItem_GetHairCardDef_005d9a10.cpp).
