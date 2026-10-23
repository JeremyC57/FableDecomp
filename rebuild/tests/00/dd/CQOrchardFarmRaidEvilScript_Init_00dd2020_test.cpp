#include <cstdio>
#include <cstring>
// Behaviour test for CQ_OrchardFarmRaidEvilScript::Init (0x00dd2020): the quest regions, boasts (with their def cost/reward), quest
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
    unsigned char _pad0[0x168];
    long OFEvilNakedBoastCost;   // +0x168
    long OFEvilNakedBoastReward;   // +0x16C
    long OFEvilNoDamageBoastCost;   // +0x170
    long OFEvilNoDamageBoastReward;   // +0x174
    long OFEvilNoWeaponsBoastCost;   // +0x178
    long OFEvilNoWeaponsBoastReward;   // +0x17C
    unsigned char _pad180[0xbf8];
    long OFE_NoHealthPotionBoastCost;   // +0xD78
    long OFE_NoHealthPotionBoastReward;   // +0xD7C
    long OFE_NoBanditsDieBoastCost;   // +0xD80
    long OFE_NoBanditsDieBoastReward;   // +0xD84
};
static CScriptDef* g_pScriptDef;                        // 0x0143E90C
struct CScriptBase {                                    // retail 0x40 bytes
    unsigned char _body[0x40];
};
struct CQ_OrchardFarmRaidEvilScript : CScriptBase {
    CGameScriptInterface* Game;                         // +0x40
    void Init();
};
void CQ_OrchardFarmRaidEvilScript::Init()
{
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, g_pScriptDef->OFEvilNakedBoastCost,
                   g_pScriptDef->OFEvilNakedBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, g_pScriptDef->OFEvilNoDamageBoastCost,
                   g_pScriptDef->OFEvilNoDamageBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NOWEAPONS", 6, g_pScriptDef->OFEvilNoWeaponsBoastCost,
                   g_pScriptDef->OFEvilNoWeaponsBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_USENOHEALTHPOTIONS", 8, g_pScriptDef->OFE_NoHealthPotionBoastCost,
                   g_pScriptDef->OFE_NoHealthPotionBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_PROTECTBANDITS", 18, g_pScriptDef->OFE_NoBanditsDieBoastCost,
                   g_pScriptDef->OFE_NoBanditsDieBoastReward, true, "", 0);
    Game->ActivateQuest("Q_OrchardFarmRaid");
}

int main()
{
    static CScriptDef def; unsigned char* p = (unsigned char*)&def;
    for (unsigned i = 0; i < sizeof(def); ++i) p[i] = (unsigned char)(i * 7 + 3);
    g_pScriptDef = &def;
    CGameScriptInterface gsi;
    CQ_OrchardFarmRaidEvilScript s; s.Game = &gsi;
    g_log[0] = 0;
    s.Init();
    char want[4096]; want[0] = 0; char b[256];
    std::sprintf(b, "B:TEXT_QST_BOAST_DESCRIPTION_NAKED:1:%ld:%ld:0::0;", (long)def.OFEvilNakedBoastCost, (long)def.OFEvilNakedBoastReward); std::strcat(want, b);
    std::sprintf(b, "B:TEXT_QST_BOAST_DESCRIPTION_NODAMAGE:3:%ld:%ld:0::0;", (long)def.OFEvilNoDamageBoastCost, (long)def.OFEvilNoDamageBoastReward); std::strcat(want, b);
    std::sprintf(b, "B:TEXT_QST_BOAST_DESCRIPTION_NOWEAPONS:6:%ld:%ld:0::0;", (long)def.OFEvilNoWeaponsBoastCost, (long)def.OFEvilNoWeaponsBoastReward); std::strcat(want, b);
    std::sprintf(b, "B:TEXT_QST_BOAST_DESCRIPTION_USENOHEALTHPOTIONS:8:%ld:%ld:0::0;", (long)def.OFE_NoHealthPotionBoastCost, (long)def.OFE_NoHealthPotionBoastReward); std::strcat(want, b);
    std::sprintf(b, "B:TEXT_QST_BOAST_DESCRIPTION_PROTECTBANDITS:18:%ld:%ld:1::0;", (long)def.OFE_NoBanditsDieBoastCost, (long)def.OFE_NoBanditsDieBoastReward); std::strcat(want, b);
    std::strcat(want, "A:Q_OrchardFarmRaid;");
    if (std::strcmp(g_log, want) != 0 ) { std::printf("00dd2020_TEST FAIL\n"); return 1; }
    std::printf("00dd2020_TEST PASS\n");
    return 0;
}