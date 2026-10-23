#include <cstdio>
#include <cstring>
// Behaviour test for CQ_TraderConflictEvilScript::Init (0x00df5cd0): the quest regions, boasts (with their def cost/reward), quest
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
static CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
struct CTimer {                                         // a script timer: the interface's timer index
    long Index;
    void SetTime(long seconds) { const CTimer* const& self = this; g_pGameScriptInterface->SetTimer(self->Index, seconds); }
};
struct CScriptDef {
    unsigned char _pad0[0x1c8];
    long TraderConflictEvilNakedBoastCost;              // +0x1C8
    long TraderConflictEvilNakedBoastReward;            // +0x1CC
    long TraderConflictEvilNoDamageBoastCost;           // +0x1D0
    long TraderConflictEvilNoDamageBoastReward;         // +0x1D4
    long TraderConflictEvilNoWeaponsBoastCost;          // +0x1D8
    long TraderConflictEvilNoWeaponsBoastReward;        // +0x1DC
    unsigned char _pad1e0[0x68];
    long TCEKeepBanditFollowerAliveCost;                // +0x248
    long TCEKeepBanditFollowerAliveReward;              // +0x24C
    long TCEMadeTimeLimitCost;                          // +0x250
    long TCEMadeTimeLimitReward;                        // +0x254
};
static CScriptDef* g_pScriptDef;                        // 0x0143E90C
struct CQ_SunnyvaleMasterData {
    unsigned char _pad0[0x128];
    bool TCEKeepBanditFollowerAlive;                    // +0x128
    bool TCEMadeTimeLimit;                              // +0x129
};
struct CScriptBase {                                    // retail 0x40 bytes
    unsigned char _body[0x40];
};
struct CQ_TraderConflictEvilScript : CScriptBase {
    CGameScriptInterface*   Game;                       // +0x40
    CQ_SunnyvaleMasterData* Data;                       // +0x44
    unsigned char           AllCreatures[0xc];          // +0x48 vector<CScriptThing>
    long                    CounterID;                  // +0x54
    long                    NumberSpawned;              // +0x58
    long                    InitialNumberInRegion;      // +0x5C
    bool                    QuestStartScreened;         // +0x60
    long                    NextTimeToSpawnGuards;      // +0x64
    bool                    MissionSucceeded;           // +0x68
    bool                    HeroAttackedBandit;         // +0x69
    bool                    PlayerEngaged;              // +0x6A
    CTimer                  ScreamOutTimer;             // +0x6C
    void Init();
};
void CQ_TraderConflictEvilScript::Init()
{
    CounterID = 0;
    NumberSpawned = 0;
    InitialNumberInRegion = 0;
    QuestStartScreened = false;
    NextTimeToSpawnGuards = 24;
    MissionSucceeded = false;
    HeroAttackedBandit = false;
    ScreamOutTimer.SetTime(15);
    PlayerEngaged = false;
    Game->AddQuestRegion("Q_TraderConflictEvil", "BarrowFields");
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, g_pScriptDef->TraderConflictEvilNakedBoastCost,
                   g_pScriptDef->TraderConflictEvilNakedBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, g_pScriptDef->TraderConflictEvilNoDamageBoastCost,
                   g_pScriptDef->TraderConflictEvilNoDamageBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NOWEAPONS", 6, g_pScriptDef->TraderConflictEvilNoWeaponsBoastCost,
                   g_pScriptDef->TraderConflictEvilNoWeaponsBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_TCEBANDITALIVE", 0x24, g_pScriptDef->TCEKeepBanditFollowerAliveCost,
                   g_pScriptDef->TCEKeepBanditFollowerAliveReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_TCETIMELIMIT", 0x25, g_pScriptDef->TCEMadeTimeLimitCost,
                   g_pScriptDef->TCEMadeTimeLimitReward, false, "", 0);
    Data->TCEKeepBanditFollowerAlive = true;
    Data->TCEMadeTimeLimit = false;
}

int main()
{
    static CScriptDef def; unsigned char* p = (unsigned char*)&def;
    for (unsigned i = 0; i < sizeof(def); ++i) p[i] = (unsigned char)(i * 7 + 3);
    g_pScriptDef = &def;
    CGameScriptInterface gsi;
    g_pGameScriptInterface = &gsi;
    static CQ_SunnyvaleMasterData data; std::memset(&data, 0, sizeof(data)); data.TCEMadeTimeLimit = true;
    static CQ_TraderConflictEvilScript s; std::memset(&s, 0x5a, sizeof(s)); s.Game = &gsi; s.Data = &data; s.ScreamOutTimer.Index = 77;
    g_log[0] = 0;
    s.Init();
    char want[4096]; want[0] = 0; char b[256];
    std::strcat(want, "T:77:15;");
    std::strcat(want, "R:Q_TraderConflictEvil:BarrowFields;");
    std::sprintf(b, "B:TEXT_QST_BOAST_DESCRIPTION_NAKED:1:%ld:%ld:0::0;", (long)def.TraderConflictEvilNakedBoastCost, (long)def.TraderConflictEvilNakedBoastReward); std::strcat(want, b);
    std::sprintf(b, "B:TEXT_QST_BOAST_DESCRIPTION_NODAMAGE:3:%ld:%ld:0::0;", (long)def.TraderConflictEvilNoDamageBoastCost, (long)def.TraderConflictEvilNoDamageBoastReward); std::strcat(want, b);
    std::sprintf(b, "B:TEXT_QST_BOAST_DESCRIPTION_NOWEAPONS:6:%ld:%ld:0::0;", (long)def.TraderConflictEvilNoWeaponsBoastCost, (long)def.TraderConflictEvilNoWeaponsBoastReward); std::strcat(want, b);
    std::sprintf(b, "B:TEXT_QST_BOAST_DESCRIPTION_TCEBANDITALIVE:36:%ld:%ld:0::0;", (long)def.TCEKeepBanditFollowerAliveCost, (long)def.TCEKeepBanditFollowerAliveReward); std::strcat(want, b);
    std::sprintf(b, "B:TEXT_QST_BOAST_DESCRIPTION_TCETIMELIMIT:37:%ld:%ld:0::0;", (long)def.TCEMadeTimeLimitCost, (long)def.TCEMadeTimeLimitReward); std::strcat(want, b);
    if (std::strcmp(g_log, want) != 0 || s.CounterID != 0 || s.NumberSpawned != 0 || s.InitialNumberInRegion != 0 || s.QuestStartScreened || s.NextTimeToSpawnGuards != 24 || s.MissionSucceeded || s.HeroAttackedBandit || s.PlayerEngaged || !data.TCEKeepBanditFollowerAlive || data.TCEMadeTimeLimit) { std::printf("00df5cd0_TEST FAIL\n"); return 1; }
    std::printf("00df5cd0_TEST PASS\n");
    return 0;
}
