// CGameScriptInterface::HeroGoFishing(bool using_fishing_spot) const -- the hero's fishing component starts fishing
// Leaf: ?HeroGoFishing@CGameScriptInterface@@UBEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00895A90. Identity (NF435, 2026-11-25): the retail RTTI vftable placement of
//   CGameScriptInterface (vtable 0x01260F0C, ghidra_out/labels_rtti_port.tsv) names this PDB member here, and Ego_r.pdb /
//   FableWin.pdb list the symbol (two sources); defined on the real class of engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT).
// FableWin's /Od body: PlayerManager (+0x14) ?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ (0x00449970), ?GetPControlledCreature@CPlayer@@QBEPAVCThingPlayerCreature@@XZ
//   (0x00487DC0), ??$IS_THING_ALIVE@PAVCThingPlayerCreature@@@@YI_NABQAVCThingPlayerCreature@@@Z, ??$GetTC@VCTCFishing@@@CThing@@QAE_NPAPAVCTCFishing@@@Z (inline in retail: bit 0xB1, LowerBound 0x0040F020), then
//   ?GoFishing@CTCFishing@@QAEX_N@Z (retail 0x007B1EB0, the call target). The DoErrorMessage branch is debug-only.
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
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CFISHINGDEF_PASS22
#define FABLE_RETAIL_CTCFISHING_PASS22
#define FABLE_RETAIL_CTCFISHING_GOFISHING
#include "engine/CGameScriptInterface.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CThing.h"
#include "engine/ThingAlive.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCFishing.h"

void CGameScriptInterface::HeroGoFishing(bool using_fishing_spot) const
{
    CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
    if (IS_THING_ALIVE(hero)) {
        CTCFishing* fishing;
        if (hero->GetTC(&fishing))
            fishing->GoFishing(using_fishing_spot);
    }
}
