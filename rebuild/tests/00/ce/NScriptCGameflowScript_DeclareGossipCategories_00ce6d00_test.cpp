// Behaviour model of NScript::CGameflowScript::DeclareGossipCategories (0x00CE6D00): 54 AddRumourCategory calls, retail order, no duplicates.
#include <cstdio>
#include <string>
#include <vector>
struct Game { std::vector<std::string> Cats; void AddRumourCategory(const std::string& s) { Cats.push_back(s); } };
static void DeclareGossipCategories(Game* G)
{
    G->AddRumourCategory("OV_INTRO");
    G->AddRumourCategory("GUILD_TRAINING");
    G->AddRumourCategory("WASP_BOSS");
    G->AddRumourCategory("DOING_WASP_BOSS");
    G->AddRumourCategory("VISIT_MAZE_1_GLOBAL");
    G->AddRumourCategory("VISIT_MAZE_1_BSSLUMS");
    G->AddRumourCategory("PRE_ORCH_FARM");
    G->AddRumourCategory("DOING_ORCH_FARM");
    G->AddRumourCategory("TRADER_ESCORT_GLOBAL");
    G->AddRumourCategory("TRADER_ESCORT_BSSLUMS");
    G->AddRumourCategory("VISIT_MAZE_2_GLOBAL");
    G->AddRumourCategory("VISIT_MAZE_2_BSSLUMS");
    G->AddRumourCategory("DOING_BANDIT_CAMP_GLOBAL");
    G->AddRumourCategory("DOING_BANDIT_CAMP_BANDITCAMP");
    G->AddRumourCategory("VISIT_MAZE_3_GLOBAL");
    G->AddRumourCategory("VISIT_MAZE_3_GUILD");
    G->AddRumourCategory("FIND_ARCHAEOLOGIST");
    G->AddRumourCategory("PRE_WHITE_BALV_GLOBAL");
    G->AddRumourCategory("PRE_WHITE_BALV_WITCHWOOD");
    G->AddRumourCategory("DOING_WHITE_BALV_GLOBAL");
    G->AddRumourCategory("DOING_WHITE_BALV_KHG");
    G->AddRumourCategory("PRE_ARENA_GLOBAL");
    G->AddRumourCategory("PRE_ARENA_KHG");
    G->AddRumourCategory("WHISPERS_FATE");
    G->AddRumourCategory("PRE_MCC_GLOBAL");
    G->AddRumourCategory("PRE_MCC_BSTONES");
    G->AddRumourCategory("DOING_MCC_GLOBAL");
    G->AddRumourCategory("DOING_GRAVEYARD_GLOBAL");
    G->AddRumourCategory("DOING_GRAVEYARD_GRAVEYARD");
    G->AddRumourCategory("HOOK_COAST_GATEWAY_GLOBAL");
    G->AddRumourCategory("HOOK_COAST_GATEWAY_DARKWOOD");
    G->AddRumourCategory("IN_HOOK_COAST_POST_DRAGON_PRE_BATTLE_GLOBAL");
    G->AddRumourCategory("IN_HOOK_COAST_POST_DRAGON_PRE_BATTLE_HOOKCOAST");
    G->AddRumourCategory("AFTER_WIZARD_BATTLE_GLOBAL");
    G->AddRumourCategory("DOING_FOCAL_SITES_GLOBAL");
    G->AddRumourCategory("DOING_FOCAL_SITES_GUILD");
    G->AddRumourCategory("FINAL_BATTLE_GLOBAL");
    G->AddRumourCategory("FINAL_BATTLE_GUILD");
    G->AddRumourCategory("AFTER_FINAL_BATTLE_GLOBAL");
    G->AddRumourCategory("AFTER_FINAL_BATTLE_KILLED_SISTER_GLOBAL");
    G->AddRumourCategory("AFTER_FINAL_BATTLE_SPARED_SISTER_GLOBAL");
    G->AddRumourCategory("AFTER_FINAL_BATTLE_GOT_SWORD_GLOBAL");
    G->AddRumourCategory("AFTER_FINAL_BATTLE_DIDNT_GET_SWORD_GLOBAL");
    G->AddRumourCategory("LOOKOUT_POINT_DEMON_DOOR_READY");
    G->AddRumourCategory("SUMMON_THE_SHIP");
    G->AddRumourCategory("NORTHERN_WASTES_OPEN");
    G->AddRumourCategory("SCARY_NECROPOLIS");
    G->AddRumourCategory("NECROPOLIS_FINISHED");
    G->AddRumourCategory("THUNDER_KILLED");
    G->AddRumourCategory("BRIAR_ROSE_KILLED");
    G->AddRumourCategory("GUILDMASTER_KILLED");
    G->AddRumourCategory("NONE_KILLED");
    G->AddRumourCategory("DRAGON_GATE_OPEN");
    G->AddRumourCategory("SNOWSPIRE_ARRIVAL");
}
int main()
{
    Game g;
    DeclareGossipCategories(&g);
    if (g.Cats.size() != 54 || g.Cats[0] != "OV_INTRO" || g.Cats[53] != "SNOWSPIRE_ARRIVAL" || g.Cats[31] != "IN_HOOK_COAST_POST_DRAGON_PRE_BATTLE_GLOBAL") { std::printf("FAIL\n"); return 1; }
    for (size_t i = 0; i < g.Cats.size(); ++i)
        for (size_t j = i + 1; j < g.Cats.size(); ++j)
            if (g.Cats[i] == g.Cats[j]) { std::printf("FAIL dup\n"); return 1; }
    std::printf("00ce6d00_TEST PASS\n");
    return 0;
}