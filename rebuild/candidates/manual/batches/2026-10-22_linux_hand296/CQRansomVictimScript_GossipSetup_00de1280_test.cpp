// behaviour test for CQ_RansomVictimScript::GossipSetup (0x00DE1280): two categories, four registrations each
#include <cstdio>
#include <cstring>
struct CCharString {
    const char* Text;
    CCharString(const char* text) : Text(text) {}
};
static char g_log[2048]; 
static void Log(const char* op, const char* a, const char* b) { std::strcat(g_log, op); std::strcat(g_log, ":"); std::strcat(g_log, a); if (b) { std::strcat(g_log, ","); std::strcat(g_log, b); } std::strcat(g_log, ";"); }
struct CGameScriptInterface {
    void AddRumourCategory(const CCharString& c) const { Log("R", c.Text, 0); }
    void AddNewRumourToCategory(const CCharString& c, const CCharString& r) const { Log("N", c.Text, r.Text); }
    void AddGossipVillage(const CCharString& c, const CCharString& v) const { Log("V", c.Text, v.Text); }
    void AddGossipFactionToCategory(const CCharString& c, const CCharString& f) const { Log("F", c.Text, f.Text); }
};
struct CQ_RansomVictimScript {
    CGameScriptInterface* GameScriptInterface;
    void GossipSetup()
    {
        GameScriptInterface->AddRumourCategory("ChiefsSonDied");
        GameScriptInterface->AddNewRumourToCategory("ChiefsSonDied", "TEXT_AI_GOSSIP_CHIEFS_SON_DIED");
        GameScriptInterface->AddGossipVillage("ChiefsSonDied", "VILLAGE_KNOTHOLE_GLADE");
        GameScriptInterface->AddGossipFactionToCategory("ChiefsSonDied", "FACTION_VILLAGERS");
        GameScriptInterface->AddRumourCategory("ChiefsSonSurvived");
        GameScriptInterface->AddNewRumourToCategory("ChiefsSonSurvived", "TEXT_AI_GOSSIP_CHIEFS_SON_SURVIVED");
        GameScriptInterface->AddGossipVillage("ChiefsSonSurvived", "VILLAGE_KNOTHOLE_GLADE");
        GameScriptInterface->AddGossipFactionToCategory("ChiefsSonSurvived", "FACTION_VILLAGERS");
    }
};
int main()
{
    CGameScriptInterface gsi; CQ_RansomVictimScript s; s.GameScriptInterface = &gsi; s.GossipSetup();
    const char* want = "R:ChiefsSonDied;N:ChiefsSonDied,TEXT_AI_GOSSIP_CHIEFS_SON_DIED;V:ChiefsSonDied,VILLAGE_KNOTHOLE_GLADE;F:ChiefsSonDied,FACTION_VILLAGERS;"
        "R:ChiefsSonSurvived;N:ChiefsSonSurvived,TEXT_AI_GOSSIP_CHIEFS_SON_SURVIVED;V:ChiefsSonSurvived,VILLAGE_KNOTHOLE_GLADE;F:ChiefsSonSurvived,FACTION_VILLAGERS;";
    if (std::strcmp(g_log, want) != 0) { std::printf("00de1280_TEST FAIL\n"); return 1; }
    std::printf("00de1280_TEST PASS\n");
    return 0;
}
