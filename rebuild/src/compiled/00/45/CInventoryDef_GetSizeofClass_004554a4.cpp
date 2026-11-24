// Leaf: ?GetSizeofClass@CInventoryDef@@UAEKXZ

#define FABLE_RETAIL_CINVENTORYDEF_RTTI_DECLS
#define FABLE_RETAIL_DEF_CLASS_BASE         // real virtual CDefClassBase / CParentDefClassBase (vfptr at +0)
#include "engine/CInventoryDef.h"
unsigned long CInventoryDef::GetSizeofClass()
{
    return 0xa34;
}
// Flatten route (flatten_pilot v4, NF324): CInventoryDef_GetSizeofClass -> CInventoryDef::GetSizeofClass, the RTTI-placed PDB member; class model: engine/CInventoryDef.h opt-in FABLE_RETAIL_CINVENTORYDEF_RTTI_DECLS (preamble of CInventoryDef_Transfer_004678e2.cpp).
