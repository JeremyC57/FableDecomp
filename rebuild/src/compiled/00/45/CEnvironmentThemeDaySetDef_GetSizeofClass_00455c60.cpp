// Leaf: ?GetSizeofClass@CEnvironmentThemeDaySetDef@@UAEKXZ
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENVIRONMENTTHEMEDAYSETDEF
#include "engine/CEnvironmentThemeDaySetDef.h"
#pragma optimize("s",on)
#include "engine/CEnvironmentThemeDaySetDef.h"  // retyped onto the PDB layout; byte parity re-verified

// Retail returns 0x6c: the retail class ends at EditorColour (+0x6c..+0x70) and does NOT
// carry the Ego_r donor's trailing FishWeightMult (+0x70), so sizeof(header) == 0x74 would
// change the immediate. Keep the retail constant, as the other landed GetSizeofClass do.
unsigned long CEnvironmentThemeDaySetDef::GetSizeofClass()
{
    return 0x6c;
}
// Flatten route (flatten_pilot v4, NF324): CEnvironmentThemeDaySetDef_GetSizeofClass -> CEnvironmentThemeDaySetDef::GetSizeofClass, the RTTI-placed PDB member; class model: engine/CEnvironmentThemeDaySetDef.h under FABLE_RETAIL_CENVIRONMENTTHEMEDAYSETDEF (preamble of CEnvironmentThemeDaySetDef_scalardeletingdestructor_00455c64.cpp).
