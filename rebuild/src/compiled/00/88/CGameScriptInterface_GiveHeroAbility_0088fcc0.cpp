// CGameScriptInterface::GiveHeroAbility(EHeroAbility ability, bool silent) const -- the main player's live controlled creature learns the ability
// Leaf: ?GiveHeroAbility@CGameScriptInterface@@UBEXW4EHeroAbility@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0088FCC0. Identity (NF433, 2026-11-25): the retail RTTI vftable placement of
//   CGameScriptInterface (vtable 0x01260F0C, ghidra_out/labels_rtti_port.tsv) names this PDB member here, and Ego_r.pdb /
//   FableWin.pdb list the symbol (two sources); defined on the real class of engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT).
// FableWin's /Od body: PlayerManager (+0x14), ?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ (retail 0x00449970),
//   ?GetPControlledCreature@CPlayer@@QBEPAVCThingPlayerCreature@@XZ (retail 0x00487DC0), ??$IS_THING_ALIVE@PAVCThingPlayerCreature@@@@YI_NABQAVCThingPlayerCreature@@@Z,
//   then ?LearnAbility@CThingPlayerCreature@@QAE_NW4EHeroAbility@@_NJ1@Z (retail 0x006AC690) with (ability, silent, -1, true).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE_LEARNABILITY
#include "engine/CGameScriptInterface.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CThing.h"
#include "engine/ThingAlive.h"
#include "engine/CThingPlayerCreature.h"

void CGameScriptInterface::GiveHeroAbility(EHeroAbility ability, bool silent) const
{
    CThingPlayerCreature* creature = PlayerManager->GetMainPlayer().GetPControlledCreature();
    if (IS_THING_ALIVE(creature))
        creature->LearnAbility(ability, silent, -1, true);
}
