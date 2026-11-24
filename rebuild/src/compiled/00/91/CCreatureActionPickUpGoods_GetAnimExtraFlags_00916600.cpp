// Leaf: ?GetAnimExtraFlags@CCreatureAction_PickUpGoods@@UBEKXZ
#define FABLE_RETAIL_CCREATUREACTION_PICKUPGOODS_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "engine/CCreatureAction_PickUpGoods.h"
unsigned long CCreatureAction_PickUpGoods::GetAnimExtraFlags() const
{
    return 8;
}
// Flatten route (flatten_pilot v5, NF330): CCreatureAction_PickUpGoods_GetAnimExtraFlags -> CCreatureAction_PickUpGoods::GetAnimExtraFlags, the RTTI-placed PDB member; class model: engine/CCreatureAction_PickUpGoods.h opt-in FABLE_RETAIL_CCREATUREACTION_PICKUPGOODS_VIRTUAL_MODEL, FABLE_RETAIL_BASE_OBJECT, FABLE_RETAIL_CCREATUREACTIONBASE.
