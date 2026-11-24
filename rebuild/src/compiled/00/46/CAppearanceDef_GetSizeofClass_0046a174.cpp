// Leaf: ?GetSizeofClass@CAppearanceDef@@UAEKXZ
#define FABLE_RETAIL_CAPPEARANCEDEF_RTTI_DECLS
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CAppearanceDef.h"
#pragma optimize("s",on)
unsigned long CAppearanceDef::GetSizeofClass()
{
    return 120;
}
// Flatten route (flatten_pilot v4, NF324): CAppearanceDef_GetSizeofClass -> CAppearanceDef::GetSizeofClass, the RTTI-placed PDB member; class model: engine/CAppearanceDef.h opt-in FABLE_RETAIL_CAPPEARANCEDEF_RTTI_DECLS (preamble of CTCLook_OnCreate_00753040.cpp).
