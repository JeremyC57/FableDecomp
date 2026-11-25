// CGameScriptInterface::CancelUsingAbility(EHeroAbility e) const -- the hero's special abilities component cancels an ability
// Leaf: ?CancelUsingAbility@CGameScriptInterface@@UBEXW4EHeroAbility@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0089D750. Identity (NF435, 2026-11-25): the retail RTTI vftable placement of
//   CGameScriptInterface (vtable 0x01260F0C, ghidra_out/labels_rtti_port.tsv) names this PDB member here, and Ego_r.pdb /
//   FableWin.pdb list the symbol (two sources); defined on the real class of engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT).
// FableWin's /Od body: PlayerManager (+0x14) ?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ (0x00449970), ?GetPControlledCreature@CPlayer@@QBEPAVCThingPlayerCreature@@XZ
//   (0x00487DC0), ??$IS_THING_ALIVE@PAVCThingPlayerCreature@@@@YI_NABQAVCThingPlayerCreature@@@Z, ??$GetTC@VCTCSpecialAbilities@@@CThing@@QAE_NPAPAVCTCSpecialAbilities@@@Z (inline in retail: bit 0xA7, LowerBound 0x0040F020), then
//   ?CancelUsingAbility@CTCSpecialAbilities@@QAE_NW4EHeroAbility@@@Z (retail 0x0072ACF0, the call target; its result is unused).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#define FABLE_RETAIL_CTCSPECIALABILITIES_GUI_DRAW_PASS23
#define FABLE_RETAIL_CTCSPECIALABILITIES_CANCELUSINGABILITY
#include "engine/CGameScriptInterface.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CThing.h"
#include "engine/ThingAlive.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCSpecialAbilities.h"

void CGameScriptInterface::CancelUsingAbility(EHeroAbility e) const
{
    CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
    if (IS_THING_ALIVE(hero)) {
        CTCSpecialAbilities* abilities;
        if (hero->GetTC(&abilities))
            abilities->CancelUsingAbility(e);
    }
}
