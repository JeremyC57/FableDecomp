#include <cstdio>
#include <cstring>
struct CCharString {
    const char* p;
    CCharString() : p("") {}
    CCharString(const char* t, long = -1) : p(t) {}
    CCharString(const CCharString& o) : p(o.p) {}
    bool is(const char* t) const { return std::strcmp(p, t) == 0; }
};
static int g_fail = 0;
static void check(bool c, int line) { if (!c) { if (!g_fail) std::printf("check failed at line %d\n", line); g_fail = 1; } }
#define CHECK(c) check((c), __LINE__)
static int g_frame = 0;
static int g_termAt = 100;
static int g_log_n = 0;
static char g_log[64][96];
static void LOG(const char* a, const char* b = "")
{
    if (g_log_n < 64) { std::strncpy(g_log[g_log_n], a, 47); g_log[g_log_n][47] = 0; std::strncat(g_log[g_log_n], b, 47); }
    ++g_log_n;
}
static int FindLog(const char* s)
{
    for (int i = 0; i < g_log_n && i < 64; ++i) if (std::strcmp(g_log[i], s) == 0) return i;
    return -1;
}
// Script timers: SetTime(t) at frame f reads back t - (frame - f).
static long g_timerSet[8], g_timerAt[8]; static int g_ntimers = 0;
struct CTimer {
    long Index;
    CTimer() { Index = g_ntimers++; g_timerSet[Index] = 0; g_timerAt[Index] = g_frame; }
    void SetTime(long t) { g_timerSet[Index] = t; g_timerAt[Index] = g_frame; }
    long GetTime() const { return g_timerSet[Index] - (g_frame - g_timerAt[Index]); }
};
struct C3DVector { float X, Y, Z; };
// Things: an id into a small world table (health > 0 means alive).
static float g_health[64];
static C3DVector g_pos[64];
static int g_nextThing = 10;
struct CScriptThing {
    int id;
    CScriptThing() : id(0) {}
    explicit CScriptThing(int i) : id(i) {}
    bool IsAlive() const { return g_health[id] > 0.0f; }
    const C3DVector& GetPos() const { return g_pos[id]; }
};
static char g_cat[8][96]; static int g_ncat = 0;
inline CCharString operator+(const CCharString& a, const CCharString& b)
{
    char* buf = g_cat[g_ncat++ & 7]; std::strcpy(buf, a.p); std::strcat(buf, b.p); return CCharString(buf);
}
inline CCharString operator+(const CCharString& a, const char* b) { return a + CCharString(b); }
struct CWideString { const wchar_t* p; CWideString(const wchar_t* t) : p(t) {} };
struct CScriptDef { long WB_ExperienceReward; };
static CScriptDef* g_pScriptDef;
static long g_xp = 0;
struct GameCalls {
    void NewScriptFrame() { ++g_frame; }
    CCharString GetActiveQuestName() { return CCharString("Q_Picnic"); }
    void SetQuestAsFailed(const CCharString& q, bool, const CWideString&, bool) { LOG("FAIL ", q.p); }
    void StopOverrideMusic(bool) { LOG("STOPMUSIC"); }
    void ResetPlayerCreatureCombatMultiplier() { LOG("RESETMULT"); }
    void GiveHeroExperience(long xp) { g_xp += xp; }
    void SetQuestAsCompleted(const CCharString& q, bool, bool, bool) { LOG("DONE ", q.p); }
    void FadeScreenIn() { LOG("FADE"); }
    bool IsLevelLoaded(const CCharString& l) { return l.is("PicnicArea") && g_frame < 8; }
    void HeroReceiveMessageFromGuildMaster(const CCharString& m, const CCharString&, bool, bool) { LOG("GM ", m.p); }
    void DeactivateQuestLater(const CCharString& q, unsigned long) { LOG("DEACT ", q.p); }
};
struct CQ_WaspBossScript {
    GameCalls* Game; bool QuestCompleted; bool QuestFailed;
    bool IsActiveThreadTerminating() { if (g_frame == 3) QuestCompleted = true; return g_frame >= g_termAt; }
    void EndMission();
};

void CQ_WaspBossScript::EndMission()
{
    while (!QuestFailed && !QuestCompleted) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    if (QuestFailed) {
        if (IsActiveThreadTerminating()) return;
        Game->SetQuestAsFailed(Game->GetActiveQuestName(), true, L"", true);
        Game->StopOverrideMusic(false);
    } else {
        if (IsActiveThreadTerminating()) return;
        Game->ResetPlayerCreatureCombatMultiplier();
        Game->GiveHeroExperience(g_pScriptDef->WB_ExperienceReward);
        Game->SetQuestAsCompleted(Game->GetActiveQuestName(), true, false, false);
        Game->StopOverrideMusic(false);
        Game->FadeScreenIn();
        while (Game->IsLevelLoaded("PicnicArea") != false) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_078_GM_MSG_FIRST", "", true, true);
    }
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}

int main()
{
    CScriptDef def; def.WB_ExperienceReward = 250; g_pScriptDef = &def;
    GameCalls g; CQ_WaspBossScript s; s.Game = &g; s.QuestCompleted = false; s.QuestFailed = false;
    s.EndMission();
    CHECK(g_xp == 250 && FindLog("DONE Q_Picnic") >= 0 && FindLog("FADE") >= 0 && FindLog("STOPMUSIC") >= 0);
    CHECK(FindLog("GM TEXT_QST_078_GM_MSG_FIRST") > FindLog("FADE") && FindLog("DEACT Q_Picnic") == g_log_n - 1 && g_frame == 8);
    g_frame = 10; g_log_n = 0; g_xp = 0; s.QuestCompleted = false; s.QuestFailed = true;
    s.EndMission();
    CHECK(g_xp == 0 && FindLog("FAIL Q_Picnic") == 0 && FindLog("STOPMUSIC") == 1 && FindLog("DEACT Q_Picnic") == 2 && g_log_n == 3);
    if (g_fail) { std::printf("00e0f1d0_TEST FAIL\n"); return 1; }
    std::printf("00e0f1d0_TEST PASS\n");
    return 0;
}
