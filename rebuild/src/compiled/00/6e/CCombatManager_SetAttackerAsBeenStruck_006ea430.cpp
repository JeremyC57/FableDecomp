// CCombatManager::SetAttackerAsBeenStruck(CThingCreatureBase& attacker) -- an attacker that is struck while in melee as an attacker has its
//   attack sequence reset
// Leaf: ?SetAttackerAsBeenStruck@CCombatManager@@QAEXAAVCThingCreatureBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006EA430, 33 bytes (`ret 4`).
// Name (NF341, 2026-11-24; B24B7 NAME_FIX): Ego_r.pdb + FableWin.pdb ?SetAttackerAsBeenStruck@CCombatManager@@QAEXAAVCThingCreatureBase@@@Z.
//   Ego_r release\combat_manager.obj record 81 (32 B) follows PerformSequenceReset (record 80) as retail 0x006EA430 follows 0x006EA2C0;
//   FableWin's body calls IsInMeleeAsAttacker then PerformSequenceReset, as this one. It was spelled DeregisterCombatant (agentNF22), which
//   is 0x006ED9C0 (FableWin's DeregisterCombatant calls DeregisterAttackingCombatant, as 0x006ED9C0 calls 0x006ED510). Before that, the
//   __fastcall stand-in CCombatManager_DeregisterCombatant(CCombatManager*, void*, CThingCreatureBase*).
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
#define FABLE_RETAIL_CCOMBATMANAGER_SETATTACKERASBEENSTRUCK
#include <new>
#include "engine/CThingCreatureBase.h"
#include "engine/CCombatManager.h"

void CCombatManager::SetAttackerAsBeenStruck(CThingCreatureBase& attacker)
{
    if (attacker.IsInMeleeAsAttacker())
        PerformSequenceReset(attacker);
}
