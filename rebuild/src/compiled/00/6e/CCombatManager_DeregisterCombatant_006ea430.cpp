// CCombatManager::DeregisterCombatant(CThingCreatureBase& creature) -- a creature leaving combat while in melee as an attacker has its
//   attack sequence reset
// Leaf: ?DeregisterCombatant@CCombatManager@@QAEXAAVCThingCreatureBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006EA430, 33 bytes (`ret 4`).
// Name (agentNF22, Codex pass 21 queue): Ego_r.pdb + FableWin.pdb ?DeregisterCombatant@CCombatManager@@QAEXAAVCThingCreatureBase@@@Z
//   (engine/CCombatManager.h declares it so). Was the __fastcall stand-in CCombatManager_DeregisterCombatant(CCombatManager*, void*,
//   CThingCreatureBase*).
// Callees: CThingCreatureBase::IsInMeleeAsAttacker() const 0x00662410 (both PDBs) and the private
//   CCombatManager::PerformSequenceReset(CThingCreatureBase&) 0x006EA2C0 (both PDBs, the landed row's name; header opt-in
//   FABLE_RETAIL_CCOMBATMANAGER_PERFORMSEQUENCERESET).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CCOMBATMANAGER_PERFORMSEQUENCERESET
#include <new>
#include "engine/CThingCreatureBase.h"
#include "engine/CCombatManager.h"

void CCombatManager::DeregisterCombatant(CThingCreatureBase& creature)
{
    if (creature.IsInMeleeAsAttacker())
        PerformSequenceReset(creature);
}
