// Leaf: ?GetSizeofClass@CBedDef@@UAEKXZ
#define FABLE_RETAIL_CBEDDEF_RTTI_DECLS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CBedDef.h"
#pragma optimize("s",on)
unsigned long CBedDef::GetSizeofClass()
{
    return 60;
}
// Flatten route (flatten_pilot v4, NF324): CBedDef_GetSizeofClass -> CBedDef::GetSizeofClass, the RTTI-placed PDB member; class model: engine/CBedDef.h opt-in FABLE_RETAIL_CBEDDEF_RTTI_DECLS (preamble of CAIStateGroupMinionSleep_OnReplace_0092cda0.cpp).
