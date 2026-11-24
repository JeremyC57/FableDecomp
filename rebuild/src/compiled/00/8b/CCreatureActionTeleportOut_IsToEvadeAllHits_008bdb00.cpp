// Leaf: ?IsToEvadeAllHits@CCreatureAction_TeleportOut@@UBE_NXZ
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_TELEPORT
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureAction_TeleportOut.h"
bool CCreatureAction_TeleportOut::IsToEvadeAllHits() const
{
    return true;
}
// Flatten route (flatten_pilot v4, NF324): CCreatureAction_TeleportOut_IsToEvadeAllHits -> CCreatureAction_TeleportOut::IsToEvadeAllHits, the RTTI-placed PDB member; class model: engine/CCreatureAction_TeleportOut.h under FABLE_RETAIL_CCREATUREACTION_TELEPORT (preamble of CCreatureActionTeleportOut_FinishAction_008bd3f0.cpp).
