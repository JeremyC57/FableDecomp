// Leaf: ?GetSizeofClass@CSnowTrollDef@@UAEKXZ
#define FABLE_RETAIL_CSNOWTROLLDEF_RTTI_DECLS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CSNOWTROLLDEF
#include "engine/CSnowTrollDef.h"
#pragma optimize("s",on)

#include "engine/CSnowTrollDef.h"  // retyped onto the PDB layout; byte parity re-verified
unsigned long CSnowTrollDef::GetSizeofClass()
{
    return 0x60;
}
// Flatten route (flatten_pilot v4, NF324): CSnowTrollDef_GetSizeofClass -> CSnowTrollDef::GetSizeofClass, the RTTI-placed PDB member; class model: engine/CSnowTrollDef.h opt-in FABLE_RETAIL_CSNOWTROLLDEF, FABLE_RETAIL_CSNOWTROLLDEF_RTTI_DECLS (preamble of CDefClassBase_GetSubDefCSnowTrollDef_008c7a70.cpp).
