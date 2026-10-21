// Behaviour test for 0x00EE07E0 (CV_TravellingHeroesScript::GossipSetup): category, rumour, village, faction expansion.
#include <cstdio>
#include <string>
struct Game {
    std::string log;
    void AddRumourCategory(const std::string& c) { log += "C(" + c + ")"; }
    void AddNewRumourToCategory(const std::string& c, const std::string& r) { log += "R(" + c + "," + r + ")"; }
    void AddGossipVillage(const std::string& c, const std::string& v) { log += "V(" + c + "," + v + ")"; }
    void AddGossipFactionToCategory(const std::string& c, const std::string& f) { log += "F(" + c + "," + f + ")"; }
};
struct Script {
    Game* GSI;
    void GossipSetup(std::string category, std::string rumour, std::string village, std::string faction)
    {
        GSI->AddRumourCategory(category);
        GSI->AddNewRumourToCategory(category, rumour);
        GSI->AddGossipVillage(category, village);
        if (faction == "FACTION_GUILD") {
            GSI->AddGossipFactionToCategory(category, "FACTION_GUILD_APPRENTICES_GOOD");
            GSI->AddGossipFactionToCategory(category, "FACTION_GUILD_SERVANTS");
            GSI->AddGossipFactionToCategory(category, "FACTION_VILLAGERS");
        } else {
            GSI->AddGossipFactionToCategory(category, faction);
        }
    }
};
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d: %s\n", __LINE__, #c); } } while (0)
int main()
{
    { Game g; Script s = { &g }; s.GossipSetup("cat", "rum", "Oakvale", "FACTION_BANDITS");
      CHECK(g.log == "C(cat)R(cat,rum)V(cat,Oakvale)F(cat,FACTION_BANDITS)"); }
    { Game g; Script s = { &g }; s.GossipSetup("g", "r", "v", "FACTION_GUILD");
      CHECK(g.log == "C(g)R(g,r)V(g,v)F(g,FACTION_GUILD_APPRENTICES_GOOD)F(g,FACTION_GUILD_SERVANTS)F(g,FACTION_VILLAGERS)"); }
    { Game g; Script s = { &g }; s.GossipSetup("g", "r", "v", ""); CHECK(g.log == "C(g)R(g,r)V(g,v)F(g,)"); }
    if (g_fail) { std::printf("00ee07e0_TEST FAIL\n"); return 1; }
    std::printf("00ee07e0_TEST PASS\n");
    return 0;
}
