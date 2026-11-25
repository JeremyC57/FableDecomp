// CGameScriptInterface::IsDPadButtonHeldForExpression(const CCharString& expression_name) const -- the hero inventory's button test for an expression
// Leaf: ?IsDPadButtonHeldForExpression@CGameScriptInterface@@UBE_NABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00895430. Identity (NF435, 2026-11-25): the retail RTTI vftable placement of
//   CGameScriptInterface (vtable 0x01260F0C, ghidra_out/labels_rtti_port.tsv) names this PDB member here, and Ego_r.pdb /
//   FableWin.pdb list the symbol (two sources); defined on the real class of engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT).
// FableWin's /Od body: PlayerManager (+0x14) ?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ (0x00449970), ?GetPControlledCreature@CPlayer@@QBEPAVCThingPlayerCreature@@XZ
//   (0x00487DC0), ??$IS_THING_ALIVE@PAVCThingPlayerCreature@@@@YI_NABQAVCThingPlayerCreature@@@Z, ??$PeekTC@$$CBVCTCInventory@@@CThing@@QBE_NPAPBVCTCInventory@@@Z
//   (inline in retail: const LowerBound 0x004365B0), then ?IsButtonHeldForExpression@CTCInventory@@QBE_NABVCCharString@@@Z (retail 0x00582A16); else false.
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
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CFADEINFADEOUTBASE_DECLS
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_ORIGINAL_COMPLETE_METHODS_PASS22
#define FABLE_RETAIL_CTCINVENTORYBASE_SELECTEDOWNED
#define FABLE_RETAIL_CTCINVENTORY_ORIGINAL_COMPLETE_CLASS_PASS22
#define FABLE_RETAIL_EXPRESSION_FRAME
#include "engine/CGameScriptInterface.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CThing.h"
#include "engine/ThingAlive.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCInventory.h"

bool CGameScriptInterface::IsDPadButtonHeldForExpression(const CCharString& expression_name) const
{
    CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
    if (IS_THING_ALIVE(hero)) {
        const CTCInventory* inventory;
        if (hero->PeekTC(&inventory))
            return inventory->IsButtonHeldForExpression(expression_name);
    }
    return false;
}
