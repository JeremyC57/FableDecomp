#include <cstdio>
#include <cstring>
// Behaviour test for CQ_TraderConflictGoodScript::Init (0x00dfa0e0): the quest regions, boasts (with their def cost/reward), quest
// activation, timer and member/master-data resets happen in retail order with retail values.
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
class CCharString { public:
    char Text[64];
    CCharString(const char* t) { std::strcpy(Text, t); }
    const CCharString& operator=(const char* t) { std::strcpy(Text, t); return *this; }
};
class CGameScriptInterface { public:
    void SetTimer(long timer, long seconds) { char b[64]; std::sprintf(b, "T:%ld:%ld", timer, seconds); Log(b); }
    void AddQuestRegion(const CCharString& q, const CCharString& r) { char b[128]; std::sprintf(b, "R:%s:%s", q.Text, r.Text); Log(b); }
    void ActivateQuest(const CCharString& q) { char b[128]; std::sprintf(b, "A:%s", q.Text); Log(b); }
    void AddBoast(const CCharString& d, long id, long cost, long reward, bool flag, const CCharString& extra, long ev)
    { char b[160]; std::sprintf(b, "B:%s:%ld:%ld:%ld:%d:%s:%ld", d.Text, id, cost, reward, (int)flag, extra.Text, ev); Log(b); }
};
class CScriptDef { public:
    unsigned char _pad0[0x1e0];
    long TraderConflictGoodNakedBoastCost;              // +0x1E0
    long TraderConflictGoodNakedBoastReward;            // +0x1E4
    long TraderConflictGoodNoDamageBoastCost;           // +0x1E8
    long TraderConflictGoodNoDamageBoastReward;         // +0x1EC
    long TraderConflictGoodNoWeaponsBoastCost;          // +0x1F0
    long TraderConflictGoodNoWeaponsBoastReward;        // +0x1F4
    unsigned char _pad1f8[0x40];
    long TCGKillNoBanditsCost;                          // +0x238
    long TCGKillNoBanditsReward;                        // +0x23C
    long TCGMadeTimeLimitCost;                          // +0x240
    long TCGMadeTimeLimitReward;                        // +0x244
};
static CScriptDef* g_pScriptDef;                        // 0x0143E90C
struct CQ_SunnyvaleMasterData {
    unsigned char _pad0[0x126];
    bool TCGKillNoBandits;                              // +0x126
    bool TCGMadeTimeLimit;                              // +0x127
};
class CScriptBase { public:                                    // retail 0x40 bytes
    unsigned char _body[0x40];
};
struct CQ_TraderConflictGoodScript : CScriptBase {
    CGameScriptInterface*   Game;                       // +0x40
    CQ_SunnyvaleMasterData* Data;                       // +0x44
    unsigned char           AllCreatures[0xc];          // +0x48 vector<CScriptThing>
    long                    TradersFollowing;           // +0x54
    long                    TradersReachedTeleporter;   // +0x58
    long                    BanditSecurityLinesSaid;    // +0x5C
    bool                    FirstRegionDead;            // +0x60
    bool                    EnteredNewRegion;           // +0x61
    bool                    IntroDone;                  // +0x62
    bool                    OutroStart;                 // +0x63
    bool                    OutroDone;                  // +0x64
    bool                    OpenedCage;                 // +0x65
    bool                    CommentedOnBanditCostume;   // +0x66
    bool                    MissionSucceeded;           // +0x67
    void Init();
};
void CQ_TraderConflictGoodScript::Init()
{
    TradersFollowing = 0;
    TradersReachedTeleporter = 0;
    BanditSecurityLinesSaid = 0;
    IntroDone = false;
    OutroStart = false;
    OutroDone = false;
    OpenedCage = false;
    FirstRegionDead = false;
    EnteredNewRegion = false;
    CommentedOnBanditCostume = false;
    MissionSucceeded = false;
    Game->AddQuestRegion("Q_TraderConflictGood", "BanditCampEntrance");
    Game->AddQuestRegion("Q_TraderConflictGood", "BanditCampCentre");
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, g_pScriptDef->TraderConflictGoodNakedBoastCost,
                   g_pScriptDef->TraderConflictGoodNakedBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, g_pScriptDef->TraderConflictGoodNoDamageBoastCost,
                   g_pScriptDef->TraderConflictGoodNoDamageBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NOWEAPONS", 6, g_pScriptDef->TraderConflictGoodNoWeaponsBoastCost,
                   g_pScriptDef->TraderConflictGoodNoWeaponsBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_TCGKILLNOONE", 0x22, g_pScriptDef->TCGKillNoBanditsCost,
                   g_pScriptDef->TCGKillNoBanditsReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_TCGTIMELIMIT", 0x23, g_pScriptDef->TCGMadeTimeLimitCost,
                   g_pScriptDef->TCGMadeTimeLimitReward, false, "", 0);
    Data->TCGKillNoBandits = true;
    Data->TCGMadeTimeLimit = false;
}

int main()
{
    static CScriptDef def; unsigned char* p = (unsigned char*)&def;
    for (unsigned i = 0; i < sizeof(def); ++i) p[i] = (unsigned char)(i * 7 + 3);
    g_pScriptDef = &def;
    CGameScriptInterface gsi;
    static CQ_SunnyvaleMasterData data; std::memset(&data, 0, sizeof(data)); data.TCGMadeTimeLimit = true;
    static CQ_TraderConflictGoodScript s; std::memset(&s, 0x5a, sizeof(s)); s.Game = &gsi; s.Data = &data;
    g_log[0] = 0;
    s.Init();
    char want[4096]; want[0] = 0; char b[256];
    std::strcat(want, "R:Q_TraderConflictGood:BanditCampEntrance;");
    std::strcat(want, "R:Q_TraderConflictGood:BanditCampCentre;");
    std::sprintf(b, "B:TEXT_QST_BOAST_DESCRIPTION_NAKED:1:%ld:%ld:0::0;", (long)def.TraderConflictGoodNakedBoastCost, (long)def.TraderConflictGoodNakedBoastReward); std::strcat(want, b);
    std::sprintf(b, "B:TEXT_QST_BOAST_DESCRIPTION_NODAMAGE:3:%ld:%ld:0::0;", (long)def.TraderConflictGoodNoDamageBoastCost, (long)def.TraderConflictGoodNoDamageBoastReward); std::strcat(want, b);
    std::sprintf(b, "B:TEXT_QST_BOAST_DESCRIPTION_NOWEAPONS:6:%ld:%ld:0::0;", (long)def.TraderConflictGoodNoWeaponsBoastCost, (long)def.TraderConflictGoodNoWeaponsBoastReward); std::strcat(want, b);
    std::sprintf(b, "B:TEXT_QST_BOAST_DESCRIPTION_TCGKILLNOONE:34:%ld:%ld:0::0;", (long)def.TCGKillNoBanditsCost, (long)def.TCGKillNoBanditsReward); std::strcat(want, b);
    std::sprintf(b, "B:TEXT_QST_BOAST_DESCRIPTION_TCGTIMELIMIT:35:%ld:%ld:0::0;", (long)def.TCGMadeTimeLimitCost, (long)def.TCGMadeTimeLimitReward); std::strcat(want, b);
    if (std::strcmp(g_log, want) != 0 || s.TradersFollowing || s.TradersReachedTeleporter || s.BanditSecurityLinesSaid || s.FirstRegionDead || s.EnteredNewRegion || s.IntroDone || s.OutroStart || s.OutroDone || s.OpenedCage || s.CommentedOnBanditCostume || s.MissionSucceeded || !data.TCGKillNoBandits || data.TCGMadeTimeLimit) { std::printf("00dfa0e0_TEST FAIL\n"); return 1; }
    std::printf("00dfa0e0_TEST PASS\n");
    return 0;
}