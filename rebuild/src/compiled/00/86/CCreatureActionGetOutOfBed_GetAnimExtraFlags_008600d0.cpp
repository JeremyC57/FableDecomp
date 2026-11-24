// Leaf: ?GetAnimExtraFlags@CCreatureAction_GetOutOfBed@@UBEKXZ
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_GETOUTOFBED
#include "engine/CThing.h"
#include "engine/CCreatureAction_GetOutOfBed.h"
unsigned long CCreatureAction_GetOutOfBed::GetAnimExtraFlags() const
{
    return 8;
}
// Flatten route (flatten_pilot v4, NF324): CCreatureAction_GetOutOfBed_GetAnimExtraFlags -> CCreatureAction_GetOutOfBed::GetAnimExtraFlags, the RTTI-placed PDB member; class model: engine/CCreatureAction_GetOutOfBed.h under FABLE_RETAIL_CCREATUREACTION_GETOUTOFBED (preamble of NCreatureMode_GetUpFromSleeping_0085f6f0.cpp).
