// NScript::CGameflowScript::DeclareGossipCategories() -- declare the 54 gameflow gossip (rumour) categories, one per gameflow stage and region (OV_INTRO .. SNOWSPIRE_ARRIVAL)
// Split: second function of the chunk 853 row 0x00CE6CF0 (see split/00ce6cf0.cpp); 0x00CE6D00, 2221 bytes, graded alone against Fable.exe
//   bytes (bin/q2.py 00ce6d00 2221): RELOCATION_MATCH, first attempt.
// Leaf: ?DeclareGossipCategories@CGameflowScript@NScript@@QAEXXZ
// Identity: Ego_r.pdb ?DeclareGossipCategories@CGameflowScript@NScript@@QAEXXZ (0x00CCE770), called only from Init; 54 calls of
//   CGameScriptInterface::AddRumourCategory(const CCharString&) with the retail strings in retail order.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CGameflowScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void Init();
    void DeclareGossipCategories();
    unsigned long CoreQuestWaiting;                         // +0x48
};
}
void NScript::CGameflowScript::DeclareGossipCategories()
{
    Game->AddRumourCategory("OV_INTRO");
    Game->AddRumourCategory("GUILD_TRAINING");
    Game->AddRumourCategory("WASP_BOSS");
    Game->AddRumourCategory("DOING_WASP_BOSS");
    Game->AddRumourCategory("VISIT_MAZE_1_GLOBAL");
    Game->AddRumourCategory("VISIT_MAZE_1_BSSLUMS");
    Game->AddRumourCategory("PRE_ORCH_FARM");
    Game->AddRumourCategory("DOING_ORCH_FARM");
    Game->AddRumourCategory("TRADER_ESCORT_GLOBAL");
    Game->AddRumourCategory("TRADER_ESCORT_BSSLUMS");
    Game->AddRumourCategory("VISIT_MAZE_2_GLOBAL");
    Game->AddRumourCategory("VISIT_MAZE_2_BSSLUMS");
    Game->AddRumourCategory("DOING_BANDIT_CAMP_GLOBAL");
    Game->AddRumourCategory("DOING_BANDIT_CAMP_BANDITCAMP");
    Game->AddRumourCategory("VISIT_MAZE_3_GLOBAL");
    Game->AddRumourCategory("VISIT_MAZE_3_GUILD");
    Game->AddRumourCategory("FIND_ARCHAEOLOGIST");
    Game->AddRumourCategory("PRE_WHITE_BALV_GLOBAL");
    Game->AddRumourCategory("PRE_WHITE_BALV_WITCHWOOD");
    Game->AddRumourCategory("DOING_WHITE_BALV_GLOBAL");
    Game->AddRumourCategory("DOING_WHITE_BALV_KHG");
    Game->AddRumourCategory("PRE_ARENA_GLOBAL");
    Game->AddRumourCategory("PRE_ARENA_KHG");
    Game->AddRumourCategory("WHISPERS_FATE");
    Game->AddRumourCategory("PRE_MCC_GLOBAL");
    Game->AddRumourCategory("PRE_MCC_BSTONES");
    Game->AddRumourCategory("DOING_MCC_GLOBAL");
    Game->AddRumourCategory("DOING_GRAVEYARD_GLOBAL");
    Game->AddRumourCategory("DOING_GRAVEYARD_GRAVEYARD");
    Game->AddRumourCategory("HOOK_COAST_GATEWAY_GLOBAL");
    Game->AddRumourCategory("HOOK_COAST_GATEWAY_DARKWOOD");
    Game->AddRumourCategory("IN_HOOK_COAST_POST_DRAGON_PRE_BATTLE_GLOBAL");
    Game->AddRumourCategory("IN_HOOK_COAST_POST_DRAGON_PRE_BATTLE_HOOKCOAST");
    Game->AddRumourCategory("AFTER_WIZARD_BATTLE_GLOBAL");
    Game->AddRumourCategory("DOING_FOCAL_SITES_GLOBAL");
    Game->AddRumourCategory("DOING_FOCAL_SITES_GUILD");
    Game->AddRumourCategory("FINAL_BATTLE_GLOBAL");
    Game->AddRumourCategory("FINAL_BATTLE_GUILD");
    Game->AddRumourCategory("AFTER_FINAL_BATTLE_GLOBAL");
    Game->AddRumourCategory("AFTER_FINAL_BATTLE_KILLED_SISTER_GLOBAL");
    Game->AddRumourCategory("AFTER_FINAL_BATTLE_SPARED_SISTER_GLOBAL");
    Game->AddRumourCategory("AFTER_FINAL_BATTLE_GOT_SWORD_GLOBAL");
    Game->AddRumourCategory("AFTER_FINAL_BATTLE_DIDNT_GET_SWORD_GLOBAL");
    Game->AddRumourCategory("LOOKOUT_POINT_DEMON_DOOR_READY");
    Game->AddRumourCategory("SUMMON_THE_SHIP");
    Game->AddRumourCategory("NORTHERN_WASTES_OPEN");
    Game->AddRumourCategory("SCARY_NECROPOLIS");
    Game->AddRumourCategory("NECROPOLIS_FINISHED");
    Game->AddRumourCategory("THUNDER_KILLED");
    Game->AddRumourCategory("BRIAR_ROSE_KILLED");
    Game->AddRumourCategory("GUILDMASTER_KILLED");
    Game->AddRumourCategory("NONE_KILLED");
    Game->AddRumourCategory("DRAGON_GATE_OPEN");
    Game->AddRumourCategory("SNOWSPIRE_ARRIVAL");
}