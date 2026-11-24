// Leaf: ?IsBlockable@CCreatureAction_SnapDragon@@UBE_NXZ
#define FABLE_RETAIL_CCREATUREACTION_SNAPDRAGON_RTTI_DECLS
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_SNAPDRAGON
#include "engine/CThing.h"
#include "engine/CCreatureAction_SnapDragon.h"
bool CCreatureAction_SnapDragon::IsBlockable() const
{
    return false;
}
// Flatten route (flatten_pilot v4, NF324): CCreatureAction_SnapDragon_IsBlockable -> CCreatureAction_SnapDragon::IsBlockable, the RTTI-placed PDB member; class model: engine/CCreatureAction_SnapDragon.h opt-in FABLE_RETAIL_CCREATUREACTION_SNAPDRAGON, FABLE_RETAIL_CCREATUREACTION_SNAPDRAGON_RTTI_DECLS (preamble of CCreatureActionDragonRearUpFull_CCreatureActionDragonRearUpFull_009531a0.cpp).
