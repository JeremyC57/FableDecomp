// Behaviour test for CGameflowScript::DeclareGossipCategories (0x00D3B410): the registration sequence.
#include <cstdio>
#include <cstring>

static char g_log[4096];
static int g_count;
static void Log(const char* op, const char* a, const char* b)
{
    std::strcat(g_log, op); std::strcat(g_log, "("); std::strcat(g_log, a);
    if (b) { std::strcat(g_log, ","); std::strcat(g_log, b); }
    std::strcat(g_log, ");");
    ++g_count;
}
struct CCharString {
    const char* Text;
    CCharString(const char* text) : Text(text) { }
};
struct CGameScriptInterface {
    void AddRumourCategory(const CCharString& c) { Log("C", c.Text, 0); }
    void AddNewRumourToCategory(const CCharString& c, const CCharString& r) { Log("R", c.Text, r.Text); }
    void AddGossipVillage(const CCharString& c, const CCharString& v) { Log("V", c.Text, v.Text); }
    void AddGossipFactionToCategory(const CCharString& c, const CCharString& f) { Log("F", c.Text, f.Text); }
};
struct CGameflowScript {
    CGameScriptInterface* GSI;
    void DeclareGossipCategories();
};
void CGameflowScript::DeclareGossipCategories()
{
    GSI->AddRumourCategory("Sweet Kid");
    GSI->AddNewRumourToCategory("Sweet Kid", "TEXT_AI_GOSSIP_SWEET_KID");
    GSI->AddGossipVillage("Sweet Kid", "VILLAGE_GUILD_COMPLEX_INSIDE");
    GSI->AddGossipFactionToCategory("Sweet Kid", "FACTION_GUILD_SERVANTS");
    GSI->AddGossipFactionToCategory("Sweet Kid", "FACTION_VILLAGERS");
    GSI->AddGossipFactionToCategory("Sweet Kid", "FACTION_GUILD_APPRENTICES_GOOD");
    GSI->AddNewRumourToCategory("Young Apprentice", "TEXT_AI_GOSSIP_YOUNG_APPRENTICE");
    GSI->AddGossipVillage("Young Apprentice", "VILLAGE_GUILD_COMPLEX_INSIDE");
    GSI->AddGossipFactionToCategory("Young Apprentice", "FACTION_GUILD_SERVANTS");
    GSI->AddNewRumourToCategory("Young Annoyance", "TEXT_AI_GOSSIP_YOUNG_ANNOYANCE");
    GSI->AddGossipVillage("Young Annoyance", "VILLAGE_GUILD_COMPLEX_INSIDE");
    GSI->AddGossipFactionToCategory("Young Annoyance", "FACTION_VILLAGERS");
    GSI->AddRumourCategory("Wannabe Hero");
    GSI->AddNewRumourToCategory("Wannabe Hero", "TEXT_AI_GOSSIP_WANNABE_HERO");
    GSI->AddGossipVillage("Wannabe Hero", "VILLAGE_GUILD_COMPLEX_INSIDE");
    GSI->AddGossipFactionToCategory("Wannabe Hero", "FACTION_GUILD_APPRENTICES_GOOD");
    GSI->AddRumourCategory("Complete Melee");
    GSI->AddNewRumourToCategory("Complete Melee", "TEXT_AI_GOSSIP_MELEE_COMPLETE");
    GSI->AddGossipVillage("Complete Melee", "VILLAGE_GUILD_COMPLEX_INSIDE");
    GSI->AddGossipFactionToCategory("Complete Melee", "FACTION_GUILD_APPRENTICES_GOOD");
    GSI->AddRumourCategory("Complete Skill");
    GSI->AddNewRumourToCategory("Complete Skill", "TEXT_AI_GOSSIP_SKILL_COMPLETE");
    GSI->AddGossipVillage("Complete Skill", "VILLAGE_GUILD_COMPLEX_INSIDE");
    GSI->AddGossipFactionToCategory("Complete Skill", "FACTION_GUILD_APPRENTICES_GOOD");
    GSI->AddRumourCategory("Complete Will");
    GSI->AddNewRumourToCategory("Complete Will", "TEXT_AI_GOSSIP_WILL_COMPLETE");
    GSI->AddGossipVillage("Complete Will", "VILLAGE_GUILD_COMPLEX_INSIDE");
    GSI->AddGossipFactionToCategory("Complete Will", "FACTION_GUILD_APPRENTICES_GOOD");
}
int main()
{
    CGameScriptInterface gsi;
    CGameflowScript s;
    s.GSI = &gsi;
    g_log[0] = 0;
    s.DeclareGossipCategories();
    if (g_count != 28
        || std::strncmp(g_log, "C(Sweet Kid);R(Sweet Kid,TEXT_AI_GOSSIP_SWEET_KID);V(Sweet Kid,VILLAGE_GUILD_COMPLEX_INSIDE);", 92) != 0
        || std::strstr(g_log, "F(Complete Will,FACTION_GUILD_APPRENTICES_GOOD);") == 0
        || std::strstr(g_log, "C(Young Apprentice)") != 0) {
        std::printf("00d3b410_TEST FAIL\n");
        return 1;
    }
    std::printf("00d3b410_TEST PASS\n");
    return 0;
}