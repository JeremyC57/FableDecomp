#include <cstdio>
struct CCountedPointerWords {
    void* Data; long* Info;
    CCountedPointerWords() : Data(0), Info(0) { }
    CCountedPointerWords(const CCountedPointerWords& o) : Data(o.Data), Info(o.Info) { if (Info) ++*Info; }
    ~CCountedPointerWords() { if (Info) --*Info; }
};
class CScriptThing { public: CCountedPointerWords PImp; };
static int g_log[8]; static int g_n = 0; static float g_value = 0; static long g_countSeen = 0;
class CGameScriptInterface { public:
    virtual ~CGameScriptInterface() { }
    virtual void SetThingHealthMultiplier(const CScriptThing&, float value, long flag) { g_log[g_n++] = 265; g_value = value; }
    virtual void SetThingFlags(const CScriptThing&, long a, long b, long c) { g_log[g_n++] = 360 + a + b + c; }
    virtual void SetThingImmortal(const CScriptThing&, long flag) { g_log[g_n++] = 469 + flag; }
    virtual void SetThingAsTarget(CScriptThing thing, long flag) { g_log[g_n++] = 844 + flag; g_countSeen = *thing.PImp.Info; }
};
struct CTimerManager { long m_index, m_time; void SetTimer(long index, long time) { m_index = index; m_time = time; } };
CTimerManager g_tm; CTimerManager* g_pScriptTimerManager = &g_tm;
struct CQuestGlobals { long NumHitsBetweenForcePushes; };
CQuestGlobals g_qg; CQuestGlobals* g_pQuestGlobals = &g_qg;
struct CQ_MinionCampScript { long m_counter; };
class CTimer { public: long TimerIndex; };
struct CBriarRose {
    CGameScriptInterface* Game; CScriptThing Me; CQ_MinionCampScript* ParentClass; long NumHitsBetweenForcePushes; CTimer SpeechTimer;
    void Init()
    {
        Game->SetThingHealthMultiplier(Me, 10000.0f, 0);
        Game->SetThingFlags(Me, 1, 0, 0);
        Game->SetThingAsTarget(Me, 0);
        Game->SetThingImmortal(Me, 0);
        NumHitsBetweenForcePushes = g_pQuestGlobals->NumHitsBetweenForcePushes;
        ParentClass->m_counter = 0;
        g_pScriptTimerManager->SetTimer(SpeechTimer.TimerIndex, 0);
    }
};
int main()
{
    long count = 1; int data = 0;
    CGameScriptInterface g; CQ_MinionCampScript q; q.m_counter = 5; g_qg.NumHitsBetweenForcePushes = 3; g_tm.m_time = -1;
    CBriarRose b; b.Game = &g; b.Me.PImp.Data = &data; b.Me.PImp.Info = &count; b.ParentClass = &q; b.SpeechTimer.TimerIndex = 12;
    b.Init();
    if (g_n != 4 || g_log[0] != 265 || g_log[1] != 361 || g_log[2] != 844 || g_log[3] != 469 || g_value != 10000.0f ||
        g_countSeen != 2 || count != 1 || b.NumHitsBetweenForcePushes != 3 || q.m_counter != 0 || g_tm.m_index != 12 || g_tm.m_time != 0)
    { std::printf("00d9e9e0_TEST FAIL\n"); return 1; }
    b.Me.PImp.Info = 0;   // keep the test's Me destructor from touching the stack count
    std::printf("00d9e9e0_TEST PASS\n");
    return 0;
}