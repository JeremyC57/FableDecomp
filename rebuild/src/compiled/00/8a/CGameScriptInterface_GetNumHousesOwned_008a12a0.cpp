// CGameScriptInterface::GetNumHousesOwned() const -- the hero's houses plus shops owned, from its hero stats component
// Leaf: ?GetNumHousesOwned@CGameScriptInterface@@UBEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008A12A0. Identity (NF435, 2026-11-25): the retail RTTI vftable placement of
//   CGameScriptInterface (vtable 0x01260F0C, ghidra_out/labels_rtti_port.tsv) names this PDB member here, and Ego_r.pdb /
//   FableWin.pdb list the symbol (two sources); defined on the real class of engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT).
// FableWin's /Od body: PlayerManager (+0x14) ?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ (0x00449970), ?PeekPControlledCreature@CPlayer@@QBEPBVCThingPlayerCreature@@XZ
//   (0x00487DD0), ??$IS_THING_ALIVE@PBVCThingPlayerCreature@@@@YI_NABQBVCThingPlayerCreature@@@Z, ??$PeekTC@$$CBVCTCHeroStats@@@CThing@@QBE_NPAPBVCTCHeroStats@@@Z
//   (inline in retail: bit 4 TCI_HERO_STATS, const LowerBound 0x004365B0), then the inline ?GetNumberOfHousesOwned@CTCHeroStats@@QBEJXZ (+0xE0) plus
//   ?GetNumberOfShopsOwned@CTCHeroStats@@QBEJXZ (+0xE4); else 0. The DoErrorMessage branch is debug-only.
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
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_SERIALISE
#define FABLE_RETAIL_CTCHEROSTATS_PERSONALITY
#include "engine/CGameScriptInterface.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CThing.h"
#include "engine/ThingAlive.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCHeroStats.h"

long CGameScriptInterface::GetNumHousesOwned() const
{
    const CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().PeekPControlledCreature();
    if (IS_THING_ALIVE(hero)) {
        const CTCHeroStats* stats;
        if (hero->PeekTC(&stats))
            return stats->GetNumberOfHousesOwned() + stats->GetNumberOfShopsOwned();
    }
    return 0;
}
