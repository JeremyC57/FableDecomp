// Leaf: ?IsToReplaceItemIfRemoved@CCreatureAction_PickupGameObject@@UBE_NXZ
#define FABLE_RETAIL_CCREATUREACTION_PICKUPGAMEOBJECT_RTTI_DECLS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "engine/CIntelligentPointer.h"
#include "engine/CCreatureActionBase.h"
#define FABLE_RETAIL_CCREATUREACTION_PICKUPGAMEOBJECT
#include "engine/CCreatureAction_PickupGameObject.h"
bool CCreatureAction_PickupGameObject::IsToReplaceItemIfRemoved() const
{
    return true;
}
// Flatten route (flatten_pilot v4, NF324): CCreatureAction_PickupGameObject_IsToReplaceItemIfRemoved -> CCreatureAction_PickupGameObject::IsToReplaceItemIfRemoved, the RTTI-placed PDB member; class model: engine/CCreatureAction_PickupGameObject.h opt-in FABLE_RETAIL_CCREATUREACTION_PICKUPGAMEOBJECT, FABLE_RETAIL_CCREATUREACTION_PICKUPGAMEOBJECT_RTTI_DECLS (preamble of CCreatureActionPickupGameObject_CCreatureActionPickupGameObject_007eb2b0.cpp).
