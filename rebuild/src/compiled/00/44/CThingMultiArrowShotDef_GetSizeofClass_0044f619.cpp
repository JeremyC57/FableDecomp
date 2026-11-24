// Leaf: ?GetSizeofClass@CThingMultiArrowShotDef@@UAEKXZ
#define FABLE_RETAIL_CTHINGMULTIARROWSHOTDEF_RTTI_DECLS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_COLLISION
#include "fable_string.h"
#include "engine/CThingMultiArrowShotDef.h"
#pragma optimize("s",on)

#include "engine/CThingMultiArrowShotDef.h"  // retyped onto the PDB layout; byte parity re-verified
unsigned long CThingMultiArrowShotDef::GetSizeofClass()
{
    return 0x54;
}
// Flatten route (flatten_pilot v4, NF324): CThingMultiArrowShotDef_GetSizeofClass -> CThingMultiArrowShotDef::GetSizeofClass, the RTTI-placed PDB member; class model: engine/CThingMultiArrowShotDef.h opt-in FABLE_RETAIL_CTCSHOTDEFINITIONBASE_COLLISION, FABLE_RETAIL_CTHINGMULTIARROWSHOTDEF_RTTI_DECLS (preamble of NThingShotCTCShotDefinitionMultiArrow_InitMovement_007bcf7f.cpp).
