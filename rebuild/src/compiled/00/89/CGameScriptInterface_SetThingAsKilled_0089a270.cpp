// CGameScriptInterface::SetThingAsKilled(const CScriptThing& entity) const -- the hero's inventory stats count the thing as killed
// Leaf: ?SetThingAsKilled@CGameScriptInterface@@UBEXABVCScriptThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0089A270. Identity (NF435, 2026-11-25): the retail RTTI vftable placement of
//   CGameScriptInterface (vtable 0x01260F0C, ghidra_out/labels_rtti_port.tsv) names this PDB member here, and Ego_r.pdb /
//   FableWin.pdb list the symbol (two sources); defined on the real class of engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT).
// FableWin's /Od body: PlayerManager (+0x14) ?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ (0x00449970), ?GetPControlledCreature@CPlayer@@QBEPAVCThingPlayerCreature@@XZ
//   (0x00487DC0), the null test, ??$GetTC@VCTCInventoryStats@@@CThing@@QAE_NPAPAVCTCInventoryStats@@@Z (inline in retail: bit 0x14, LowerBound
//   0x0040F020), then ?SetCreatureAsKilled@CTCInventoryStats@@QAEXPAVCThing@@@Z (retail 0x005D66BA) with entity.GetPThing() (slot 11, +0x2c).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYSTATS
#define FABLE_RETAIL_CTCINVENTORYSTATS_SETCREATUREASKILLED
#include "engine/CGameScriptInterface.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CThing.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCInventoryStats.h"

void CGameScriptInterface::SetThingAsKilled(const CScriptThing& entity) const
{
    CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
    if (hero) {
        CTCInventoryStats* stats;
        if (hero->GetTC(&stats))
            stats->SetCreatureAsKilled(entity.GetPThing());
    }
}
