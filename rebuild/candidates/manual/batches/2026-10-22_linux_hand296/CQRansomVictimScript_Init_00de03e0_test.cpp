#include <cstdio>
#include <cstring>
// Behaviour test for CQ_RansomVictimScript::Init (0x00de03e0): the quest regions, boasts (with their def cost/reward), quest
// activation, timer and member/master-data resets happen in retail order with retail values.
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct CCharString {
    char Text[64];
    CCharString() { Text[0] = 0; }
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
    unsigned char _pad0[0x1b0];
    long RansomVictimNakedBoastCost;                    // +0x1B0
    long RansomVictimNakedBoastReward;                  // +0x1B4
    long RansomVictimNoDamageBoastCost;                 // +0x1B8
    long RansomVictimNoDamageBoastReward;               // +0x1BC
    long RansomVictimNoWeaponsBoastCost;                // +0x1C0
    long RansomVictimNoWeaponsBoastReward;              // +0x1C4
    unsigned char _pad1c8[0x90];
    long RansomVictimVictimNoDamageCost;                // +0x258
    long RansomVictimVictimNoDamageReward;              // +0x25C
    long RansomVictimKillKidnapperCost;                 // +0x260
    long RansomVictimKillKidnapperReward;               // +0x264
};
static CScriptDef* g_pScriptDef;                        // 0x0143E90C
struct CQ_SunnyvaleMasterData {
    unsigned char _pad0[0x12a];
    bool RansomVictimVictimNoDamage;                    // +0x12A
    bool RansomVictimKidnappersKilled;                  // +0x12B
    bool RansomVictimSaveVictim;                        // +0x12C
    bool RansomVictimHaveVictimKilled;                  // +0x12D
};
struct CScriptBase {                                    // retail 0x40 bytes
    unsigned char _body[0x40];
};
struct CQ_RansomVictimScript : CScriptBase {
    CGameScriptInterface*   Game;                       // +0x40
    CQ_SunnyvaleMasterData* Data;                       // +0x44
    bool                    QuestStartScreened;         // +0x48
    bool                    MissionSucceeded;           // +0x49
    bool                    MissionFailed;              // +0x4A
    bool                    RescuedVictim;              // +0x4B
    bool                    RansommerDied;              // +0x4C
    bool                    RansomHelperStarted;        // +0x4D
    bool                    RansomHelperSpoken;         // +0x4E
    bool                    ExtraAssistanceGiven;       // +0x4F
    bool                    RansomHelpersAttacked;      // +0x50
    bool                    ToldGuardToWaitOutside;     // +0x51
    bool                    EnteredNewRegion;           // +0x52
    bool                    ThanksCutsceneStarting;     // +0x53
    long                    HitChief;                   // +0x54
    long                    HitEvilBrotherAndProtection; // +0x58
    bool                    SomeoneDealingWithBeingHit; // +0x5C
    long                    KidnappersKilled;           // +0x60
    CCharString             ActiveEnemyName;            // +0x64
    void Init();
};
void CQ_RansomVictimScript::Init()
{
    QuestStartScreened = false;
    MissionFailed = false;
    MissionSucceeded = false;
    RescuedVictim = false;
    RansommerDied = false;
    RansomHelperSpoken = false;
    RansomHelperStarted = false;
    ExtraAssistanceGiven = false;
    RansomHelpersAttacked = false;
    EnteredNewRegion = false;
    ToldGuardToWaitOutside = false;
    ThanksCutsceneStarting = false;
    HitEvilBrotherAndProtection = 0;
    SomeoneDealingWithBeingHit = false;
    KidnappersKilled = 0;
    HitChief = 0;
    Game->AddQuestRegion("Q_RansomVictim", "KnotholeGlade");
    Game->AddQuestRegion("Q_RansomVictim", "Witchwood4");
    Game->AddQuestRegion("Q_RansomVictim", "Witchwood3");
    Game->AddQuestRegion("Q_RansomVictim", "Witchwood2");
    Game->AddQuestRegion("Q_RansomVictim", "WitchwoodCavern");
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, g_pScriptDef->RansomVictimNakedBoastCost,
                   g_pScriptDef->RansomVictimNakedBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, g_pScriptDef->RansomVictimNoDamageBoastCost,
                   g_pScriptDef->RansomVictimNoDamageBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NOWEAPONS", 6, g_pScriptDef->RansomVictimNoWeaponsBoastCost,
                   g_pScriptDef->RansomVictimNoWeaponsBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_RVNODAMAGE", 0x26, g_pScriptDef->RansomVictimVictimNoDamageCost,
                   g_pScriptDef->RansomVictimVictimNoDamageReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_RVKILLKIDNAPPERS", 0x27, g_pScriptDef->RansomVictimKillKidnapperCost,
                   g_pScriptDef->RansomVictimKillKidnapperReward, false, "", 0);
    Data->RansomVictimVictimNoDamage = true;
    Data->RansomVictimSaveVictim = false;
    Data->RansomVictimHaveVictimKilled = false;
    Data->RansomVictimKidnappersKilled = false;
    ActiveEnemyName = "RandomAttacker";
}

int main()
{
    static CScriptDef def; unsigned char* p = (unsigned char*)&def;
    for (unsigned i = 0; i < sizeof(def); ++i) p[i] = (unsigned char)(i * 7 + 3);
    g_pScriptDef = &def;
    CGameScriptInterface gsi;
    static CQ_SunnyvaleMasterData data; std::memset(&data, 1, sizeof(data)); data.RansomVictimVictimNoDamage = false;
    static CQ_RansomVictimScript s; std::memset(&s, 0x5a, sizeof(s)); s.Game = &gsi; s.Data = &data;
    g_log[0] = 0;
    s.Init();
    char want[4096]; want[0] = 0; char b[256];
    std::strcat(want, "R:Q_RansomVictim:KnotholeGlade;");
    std::strcat(want, "R:Q_RansomVictim:Witchwood4;");
    std::strcat(want, "R:Q_RansomVictim:Witchwood3;");
    std::strcat(want, "R:Q_RansomVictim:Witchwood2;");
    std::strcat(want, "R:Q_RansomVictim:WitchwoodCavern;");
    std::sprintf(b, "B:TEXT_QST_BOAST_DESCRIPTION_NAKED:1:%ld:%ld:0::0;", (long)def.RansomVictimNakedBoastCost, (long)def.RansomVictimNakedBoastReward); std::strcat(want, b);
    std::sprintf(b, "B:TEXT_QST_BOAST_DESCRIPTION_NODAMAGE:3:%ld:%ld:0::0;", (long)def.RansomVictimNoDamageBoastCost, (long)def.RansomVictimNoDamageBoastReward); std::strcat(want, b);
    std::sprintf(b, "B:TEXT_QST_BOAST_DESCRIPTION_NOWEAPONS:6:%ld:%ld:0::0;", (long)def.RansomVictimNoWeaponsBoastCost, (long)def.RansomVictimNoWeaponsBoastReward); std::strcat(want, b);
    std::sprintf(b, "B:TEXT_QST_BOAST_DESCRIPTION_RVNODAMAGE:38:%ld:%ld:0::0;", (long)def.RansomVictimVictimNoDamageCost, (long)def.RansomVictimVictimNoDamageReward); std::strcat(want, b);
    std::sprintf(b, "B:TEXT_QST_BOAST_DESCRIPTION_RVKILLKIDNAPPERS:39:%ld:%ld:0::0;", (long)def.RansomVictimKillKidnapperCost, (long)def.RansomVictimKillKidnapperReward); std::strcat(want, b);
    if (std::strcmp(g_log, want) != 0 || s.QuestStartScreened || s.MissionSucceeded || s.MissionFailed || s.RescuedVictim || s.RansommerDied || s.RansomHelperStarted || s.RansomHelperSpoken || s.ExtraAssistanceGiven || s.RansomHelpersAttacked || s.ToldGuardToWaitOutside || s.EnteredNewRegion || s.ThanksCutsceneStarting || s.HitChief || s.HitEvilBrotherAndProtection || s.SomeoneDealingWithBeingHit || s.KidnappersKilled || std::strcmp(s.ActiveEnemyName.Text, "RandomAttacker") || !data.RansomVictimVictimNoDamage || data.RansomVictimKidnappersKilled || data.RansomVictimSaveVictim || data.RansomVictimHaveVictimKilled) { std::printf("00de03e0_TEST FAIL\n"); return 1; }
    std::printf("00de03e0_TEST PASS\n");
    return 0;
}
