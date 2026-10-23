#include <cstdio>
#include <cstring>
// Behaviour test for CQ_OrchardFarmRaidGoodScript::Init (0x00dd23a0): the quest regions, boasts (with their def cost/reward), quest
// activation, timer and member/master-data resets happen in retail order with retail values.
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct CCharString {
    char Text[64];
    CCharString(const char* t) { std::strcpy(Text, t); }
    CCharString& operator=(const char* t) { std::strcpy(Text, t); return *this; }
};
struct CGameScriptInterface {
    void SetTimer(long timer, long seconds) { char b[64]; std::sprintf(b, "T:%ld:%ld", timer, seconds); Log(b); }
    void AddQuestRegion(const CCharString& q, const CCharString& r) { char b[128]; std::sprintf(b, "R:%s:%s", q.Text, r.Text); Log(b); }
    void ActivateQuest(const CCharString& q) { char b[128]; std::sprintf(b, "A:%s", q.Text); Log(b); }
    void AddBoast(const CCharString& d, long id, long cost, long reward, bool flag, const CCharString& extra, long ev)
    { char b[160]; std::sprintf(b, "B:%s:%ld:%ld:%ld:%d:%s:%ld", d.Text, id, cost, reward, (int)flag, extra.Text, ev); Log(b); }
};
struct CScriptDef {
    unsigned char _pad0[0x180];
    long OFGoodNakedBoastCost;   // +0x180
    long OFGoodNakedBoastReward;   // +0x184
    long OFGoodNoDamageBoastCost;   // +0x188
    long OFGoodNoDamageBoastReward;   // +0x18C
    long OFGoodNoWeaponsBoastCost;   // +0x190
    long OFGoodNoWeaponsBoastReward;   // +0x194
    unsigned char _pad198[0xbf0];
    long OFG_NoCratesStolenBoastCost;   // +0xD88
    long OFG_NoCratesStolenBoastReward;   // +0xD8C
    long OFG_NoGuardsDieBoastCost;   // +0xD90
    long OFG_NoGuardsDieBoastReward;   // +0xD94
};
static CScriptDef* g_pScriptDef;                        // 0x0143E90C
struct CScriptBase {                                    // retail 0x40 bytes
    unsigned char _body[0x40];
};
struct CQ_OrchardFarmRaidGoodScript : CScriptBase {
    CGameScriptInterface* Game;                         // +0x40
    void Init();
};
void CQ_OrchardFarmRaidGoodScript::Init()
{
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, g_pScriptDef->OFGoodNakedBoastCost,
                   g_pScriptDef->OFGoodNakedBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, g_pScriptDef->OFGoodNoDamageBoastCost,
                   g_pScriptDef->OFGoodNoDamageBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NOWEAPONS", 6, g_pScriptDef->OFGoodNoWeaponsBoastCost,
                   g_pScriptDef->OFGoodNoWeaponsBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NOCRATESSTOLEN", 17, g_pScriptDef->OFG_NoCratesStolenBoastCost,
                   g_pScriptDef->OFG_NoCratesStolenBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_PROTECTGUARDS", 19, g_pScriptDef->OFG_NoGuardsDieBoastCost,
                   g_pScriptDef->OFG_NoGuardsDieBoastReward, false, "", 0);
    Game->ActivateQuest("Q_OrchardFarmRaid");
}

int main()
{
    static CScriptDef def; unsigned char* p = (unsigned char*)&def;
    for (unsigned i = 0; i < sizeof(def); ++i) p[i] = (unsigned char)(i * 7 + 3);
    g_pScriptDef = &def;
    CGameScriptInterface gsi;
    CQ_OrchardFarmRaidGoodScript s; s.Game = &gsi;
    g_log[0] = 0;
    s.Init();
    char want[4096]; want[0] = 0; char b[256];
    std::sprintf(b, "B:TEXT_QST_BOAST_DESCRIPTION_NAKED:1:%ld:%ld:0::0;", (long)def.OFGoodNakedBoastCost, (long)def.OFGoodNakedBoastReward); std::strcat(want, b);
    std::sprintf(b, "B:TEXT_QST_BOAST_DESCRIPTION_NODAMAGE:3:%ld:%ld:0::0;", (long)def.OFGoodNoDamageBoastCost, (long)def.OFGoodNoDamageBoastReward); std::strcat(want, b);
    std::sprintf(b, "B:TEXT_QST_BOAST_DESCRIPTION_NOWEAPONS:6:%ld:%ld:0::0;", (long)def.OFGoodNoWeaponsBoastCost, (long)def.OFGoodNoWeaponsBoastReward); std::strcat(want, b);
    std::sprintf(b, "B:TEXT_QST_BOAST_DESCRIPTION_NOCRATESSTOLEN:17:%ld:%ld:0::0;", (long)def.OFG_NoCratesStolenBoastCost, (long)def.OFG_NoCratesStolenBoastReward); std::strcat(want, b);
    std::sprintf(b, "B:TEXT_QST_BOAST_DESCRIPTION_PROTECTGUARDS:19:%ld:%ld:0::0;", (long)def.OFG_NoGuardsDieBoastCost, (long)def.OFG_NoGuardsDieBoastReward); std::strcat(want, b);
    std::strcat(want, "A:Q_OrchardFarmRaid;");
    if (std::strcmp(g_log, want) != 0 ) { std::printf("00dd23a0_TEST FAIL\n"); return 1; }
    std::printf("00dd23a0_TEST PASS\n");
    return 0;
}
