// Leaf: ?GetSizeofClass@CCreatureStatsDef@@UAEKXZ
#define FABLE_RETAIL_CCREATURESTATSDEF_RTTI_DECLS
#define FABLE_RETAIL_CCREATUREDEF_TRANSFER
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CCreatureStatsDef.h"
#pragma optimize("s",on)
#include "engine/CCreatureStatsDef.h"  // retyped onto the PDB layout; byte parity re-verified
unsigned long CCreatureStatsDef::GetSizeofClass()
{
    return 0x48;
}
// Flatten route (flatten_pilot v4, NF324): CCreatureStatsDef_GetSizeofClass -> CCreatureStatsDef::GetSizeofClass, the RTTI-placed PDB member; class model: engine/CCreatureStatsDef.h opt-in FABLE_RETAIL_CCREATUREDEF_TRANSFER, FABLE_RETAIL_CCREATURESTATSDEF_RTTI_DECLS (preamble of CCreatureDef_Transfer_00675e20.cpp).
