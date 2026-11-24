// Leaf: ?GetAnimExtraFlags@CCreatureAction_PickUpGenericBox@@UBEKXZ
#define FABLE_RETAIL_CCREATUREACTION_PICKUPGENERICBOX_RTTI_DECLS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_GENERICBOX
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureAction_PickUpGenericBox.h"
unsigned long CCreatureAction_PickUpGenericBox::GetAnimExtraFlags() const
{
    return 8;
}
// Flatten route (flatten_pilot v4, NF324): CCreatureAction_PickUpGenericBox_GetAnimExtraFlags -> CCreatureAction_PickUpGenericBox::GetAnimExtraFlags, the RTTI-placed PDB member; class model: engine/CCreatureAction_PickUpGenericBox.h opt-in FABLE_RETAIL_CCREATUREACTION_GENERICBOX, FABLE_RETAIL_CCREATUREACTION_PICKUPGENERICBOX_RTTI_DECLS (preamble of CCreatureActionPickUpGenericBox_PerformAction_00848b70.cpp).
