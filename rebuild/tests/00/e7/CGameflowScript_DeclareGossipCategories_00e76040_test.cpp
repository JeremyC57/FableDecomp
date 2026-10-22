// behaviour test for CGameflowScript::DeclareGossipCategories (0x00E76040): three categories, four registrations each
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
struct CGameflowScript {
    CGameScriptInterface* GameScriptInterface;
    void DeclareGossipCategories()
    {
        GameScriptInterface->AddRumourCategory("ExposedMayor");
        GameScriptInterface->AddNewRumourToCategory("ExposedMayor", "TEXT_AI_GOSSIP_MAYOR_EXPOSED");
        GameScriptInterface->AddGossipVillage("ExposedMayor", "VILLAGE_BOWERSTONE_POSH");
        GameScriptInterface->AddGossipFactionToCategory("ExposedMayor", "FACTION_VILLAGERS");
        GameScriptInterface->AddRumourCategory("ExposedMayorNotBecomeMayor");
        GameScriptInterface->AddNewRumourToCategory("ExposedMayorNotBecomeMayor", "TEXT_AI_GOSSIP_MAYOR_EXPOSED_NOT_BECOME");
        GameScriptInterface->AddGossipVillage("ExposedMayorNotBecomeMayor", "VILLAGE_BOWERSTONE_POSH");
        GameScriptInterface->AddGossipFactionToCategory("ExposedMayorNotBecomeMayor", "FACTION_VILLAGERS");
        GameScriptInterface->AddRumourCategory("BribedMayor");
        GameScriptInterface->AddNewRumourToCategory("BribedMayor", "TEXT_AI_GOSSIP_MAYOR_BRIBED");
        GameScriptInterface->AddGossipVillage("BribedMayor", "VILLAGE_BOWERSTONE_POSH");
        GameScriptInterface->AddGossipFactionToCategory("BribedMayor", "FACTION_VILLAGERS");
    }
};
int main()
{
    CGameScriptInterface gsi; CGameflowScript s; s.GameScriptInterface = &gsi; s.DeclareGossipCategories();
    const char* want = "R:ExposedMayor;N:ExposedMayor,TEXT_AI_GOSSIP_MAYOR_EXPOSED;V:ExposedMayor,VILLAGE_BOWERSTONE_POSH;F:ExposedMayor,FACTION_VILLAGERS;"
        "R:ExposedMayorNotBecomeMayor;N:ExposedMayorNotBecomeMayor,TEXT_AI_GOSSIP_MAYOR_EXPOSED_NOT_BECOME;V:ExposedMayorNotBecomeMayor,VILLAGE_BOWERSTONE_POSH;F:ExposedMayorNotBecomeMayor,FACTION_VILLAGERS;"
        "R:BribedMayor;N:BribedMayor,TEXT_AI_GOSSIP_MAYOR_BRIBED;V:BribedMayor,VILLAGE_BOWERSTONE_POSH;F:BribedMayor,FACTION_VILLAGERS;";
    if (std::strcmp(g_log, want) != 0) { std::printf("00e76040_TEST FAIL\n"); return 1; }
    std::printf("00e76040_TEST PASS\n");
    return 0;
}