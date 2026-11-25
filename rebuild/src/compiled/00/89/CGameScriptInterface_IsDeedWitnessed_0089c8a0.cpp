// CGameScriptInterface::IsDeedWitnessed(long deed_handle) const -- the hero's opinion deed log reports whether a deed was witnessed
// Leaf: ?IsDeedWitnessed@CGameScriptInterface@@UBE_NJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0089C8A0. Identity (NF435, 2026-11-25): the retail RTTI vftable placement of
//   CGameScriptInterface (vtable 0x01260F0C, ghidra_out/labels_rtti_port.tsv) names this PDB member here, and Ego_r.pdb /
//   FableWin.pdb list the symbol (two sources); defined on the real class of engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT).
// FableWin's /Od body: PlayerManager (+0x14) ?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ (0x00449970), ?GetPControlledCreature@CPlayer@@QBEPAVCThingPlayerCreature@@XZ
//   (0x00487DC0), ??$IS_THING_ALIVE@PAVCThingPlayerCreature@@@@YI_NABQAVCThingPlayerCreature@@@Z, ??$GetTC@VCTCHeroOpinionDeedLog@@@CThing@@QAE_NPAPAVCTCHeroOpinionDeedLog@@@Z (inline in retail: bit 0x51, LowerBound 0x0040F020), then
//   ?IsDeedWitnessed@CTCHeroOpinionDeedLog@@QAE_NJ_N@Z (retail 0x006C5E90, the call target) with (deed_handle, true); else false.
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
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_ISDEEDWITNESSED
#include "engine/CGameScriptInterface.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CThing.h"
#include "engine/ThingAlive.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCHeroOpinionDeedLog.h"

bool CGameScriptInterface::IsDeedWitnessed(long deed_handle) const
{
    CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
    if (IS_THING_ALIVE(hero)) {
        CTCHeroOpinionDeedLog* deed_log;
        if (hero->GetTC(&deed_log))
            return deed_log->IsDeedWitnessed(deed_handle, true);
    }
    return false;
}
