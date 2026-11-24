// Leaf: ?GetSizeofClass@CSkyDef@@UAEKXZ
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CSKYDEF_RTTI_DECLS
#include "engine/CSkyDef.h"
unsigned long CSkyDef::GetSizeofClass()
{
    return 0x80;
}
// Flatten route (flatten_pilot v4, NF324): CSkyDef_GetSizeofClass -> CSkyDef::GetSizeofClass, the RTTI-placed PDB member; class model: engine/CSkyDef.h opt-in FABLE_RETAIL_DEF_CLASS_BASE, FABLE_RETAIL_CSKYDEF_RTTI_DECLS.
