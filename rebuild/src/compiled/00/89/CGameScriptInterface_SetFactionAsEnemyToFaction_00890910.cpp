// CGameScriptInterface::SetFactionAsEnemyToFaction(const CCharString& faction_name_1, const CCharString& faction_name_2) const -- both factions' SetAsEnemyOfFaction, each with the other
// Leaf: ?SetFactionAsEnemyToFaction@CGameScriptInterface@@UBEXABVCCharString@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00890910. Identity (NF433, 2026-11-25): the retail RTTI vftable placement of
//   CGameScriptInterface (vtable 0x01260F0C, ghidra_out/labels_rtti_port.tsv) names this PDB member here, and Ego_r.pdb /
//   FableWin.pdb list the symbol (two sources); defined on the real class of engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT).
// FableWin's /Od body: the inline ?GetFactionManager@CWorld@@QAEAAVCFactionManager@@XZ (World +0x54) and
//   ?GetPFaction@CFactionManager@@QBEPAVCFaction@@ABVCCharString@@@Z (retail 0x006B93D0) for each name, then, when both are found,
//   ?SetAsEnemyOfFaction@CFaction@@QAEXABV1@@Z (retail 0x006B9530) on the first with the second and on the second with the first. Its two
//   CDebugManager::DoErrorMessage blocks (a faction not found) are debug-only (absent in retail).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_GETFACTIONMANAGER
#define FABLE_RETAIL_CFACTION_SCRIPTRELATIONS
#include "engine/CGameScriptInterface.h"
#include "fable_string.h"
#include "engine/CWorld.h"
#include "engine/CFactionManager.h"
#include "engine/CFaction.h"

void CGameScriptInterface::SetFactionAsEnemyToFaction(const CCharString& faction_name_1, const CCharString& faction_name_2) const
{
    CFaction* faction_1 = World->GetFactionManager().GetPFaction(faction_name_1);
    CFaction* faction_2 = World->GetFactionManager().GetPFaction(faction_name_2);
    if (faction_1 && faction_2)
    {
        faction_1->SetAsEnemyOfFaction(*faction_2);
        faction_2->SetAsEnemyOfFaction(*faction_1);
    }
}
