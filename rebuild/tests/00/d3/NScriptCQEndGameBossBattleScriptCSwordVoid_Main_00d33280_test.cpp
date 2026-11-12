// Behaviour test for 0x00D33280 NScript::CQ_EndGameBossBattleScript::CSwordVoid::Main(): the row's body copied over test doubles
// (predicate, timer, handle, movie, pause object, cast map, game interface, quest flags); checks the call order of each path.
#include <cstdio>
#include <cstring>
#include <map>
#include <string>
static int g_log[512]; static int g_n = 0;
static void Log(int v) { if (g_n < 512) g_log[g_n++] = v; }
class CCharString { public: std::string s; CCharString(const char* t) : s(t) { } bool operator<(const CCharString& o) const { return s < o.s; } };
class CScriptThing { public: int id; };
struct CIsThingAlive { CScriptThing Thing; CIsThingAlive(const CScriptThing& t) : Thing(t) { Log(1); } ~CIsThingAlive() { Log(2); } };
static long g_timerValue = 0;
class CTimer { public:
    CTimer() { Log(3); }
    ~CTimer() { Log(4); }
    CTimer& operator=(long t) { Log(t == 2 ? 5 : (t == 10 ? 6 : 99)); g_timerValue = t; return *this; }
    long GetTime() const { return g_timerValue; }
};
static bool g_valid = false;
class CScriptGameResourceObjectScriptedThingBase { public:
    CScriptGameResourceObjectScriptedThingBase() { Log(10); }
    ~CScriptGameResourceObjectScriptedThingBase() { Log(11); }
    bool IsValid() const { return g_valid; }
    void Release() { Log(12); }
};
class CScriptGameResourceObjectMovieBase { public: CScriptGameResourceObjectMovieBase() { Log(13); } ~CScriptGameResourceObjectMovieBase() { Log(14); } };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_HIGHEST = 4 };
static bool g_near = false;
static long g_answers[8]; static int g_answerIdx = 0;
class CGame { public:
    CScriptThing hero;
    void NewScriptFrame() const { Log(20); --g_timerValue; }
    void MiniMapAddMarker(const CScriptThing&, const CCharString& m) const { Log(m.s == "HUD_ORB_QUEST_CORE" ? 21 : 99); }
    CScriptThing& GetHero() { return hero; }
    void GiveHeroYesNoQuestion(const CCharString& q, const CCharString& y, const CCharString& n, const CCharString& o, bool b) const
    { Log(q.s == "TEXT_QST_039_SWORD_QUESTION" && y.s == "TEXT_OBJECT_HERO_ANSWER_YES" && n.s == "TEXT_OBJECT_HERO_ANSWER_NO" && o.s.empty() && !b ? 22 : 99); }
    long MsgIsQuestionAnsweredYesOrNo() const { Log(23); return g_answers[g_answerIdx++]; }
    bool StartScriptingEntity(const CScriptThing& t, CScriptGameResourceObjectScriptedThingBase&, EScriptAIPriority p) { Log(t.id == 7 && p == 4 ? 24 : 99); return true; }
    void StartMovieSequence(const CCharString& n, CScriptGameResourceObjectMovieBase&) { Log(n.s.empty() ? 25 : 99); }
    void PauseAllNonScriptedEntities(bool b) { Log(b ? 26 : 27); }
};
struct CWideScreenMagicPauseEntities {
    CGame* Game;
    CWideScreenMagicPauseEntities(CGame& g) : Game(&g) { Game->PauseAllNonScriptedEntities(true); }
    ~CWideScreenMagicPauseEntities() { Game->PauseAllNonScriptedEntities(false); }
};
static bool IsDistanceBetweenThingsUnder(const CScriptThing&, const CScriptThing&, float d) { Log(d == 3.0f ? 28 : 99); return g_near; }
static void RunCutsceneMacro(const CCharString& n, std::map<CCharString, CScriptGameResourceObjectScriptedThingBase*>& cast, void*, void*, bool a, bool b)
{ Log(n.s == "CS_JACK_BOSS_ENDING_GOOD" && cast.size() == 1 && !a && b ? 29 : 99); }
struct CQ_EndGameBossBattleScript { bool FinalDecision, FinalDecisionMade, FinalCutscenePlayed; };
struct CQ_SunnyvaleMasterData { long JackBossBattleResult; };
static int g_doneAfter = 0, g_doneCalls = 0;
struct CSwordVoid {
    CGame* Game; CScriptThing Me; CQ_EndGameBossBattleScript& ParentClass; CQ_SunnyvaleMasterData* PMasterData;
    CSwordVoid(CQ_EndGameBossBattleScript& p) : ParentClass(p) { }
    void SetPredicate(const CIsThingAlive&) { Log(30); }
    bool IsActiveThreadTerminating() const { Log(31); return ++g_doneCalls >= g_doneAfter; }
    void Main();
};
void CSwordVoid::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    while (!ParentClass.FinalDecision) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->MiniMapAddMarker(Me, "HUD_ORB_QUEST_CORE");
    CTimer timer;
    timer = 2;
    while (!ParentClass.FinalDecisionMade) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        while (!((IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 3.0f) && timer.GetTime() <= 0) || ParentClass.FinalDecisionMade)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        if (ParentClass.FinalDecisionMade)
            continue;
        if (IsActiveThreadTerminating())
            return;
        timer = 10;
        Game->GiveHeroYesNoQuestion("TEXT_QST_039_SWORD_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", false);
        long answer = Game->MsgIsQuestionAnsweredYesOrNo();
        while (answer < 0) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            answer = Game->MsgIsQuestionAnsweredYesOrNo();
        }
        if (IsActiveThreadTerminating())
            return;
        if (answer != 1)
            continue;
        if (IsActiveThreadTerminating())
            return;
        ParentClass.FinalDecisionMade = true;
        PMasterData->JackBossBattleResult = 2;
        CScriptGameResourceObjectScriptedThingBase hero;
        if (hero.IsValid())
            hero.Release();
        while (!Game->StartScriptingEntity(Game->GetHero(), hero, SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        CScriptGameResourceObjectMovieBase movie;
        Game->StartMovieSequence("", movie);
        CWideScreenMagicPauseEntities pause(*Game);
        {
            std::map<CCharString, CScriptGameResourceObjectScriptedThingBase*> cast;
            cast["Hero"] = &hero;
            RunCutsceneMacro("CS_JACK_BOSS_ENDING_GOOD", cast, 0, 0, false, true);
            ParentClass.FinalCutscenePlayed = true;
        }
    }
    if (IsActiveThreadTerminating())
        return;
    do {
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}
int main()
{
    bool ok = true;
    {   // terminating after the first frame: only the predicate and one frame
        CGame game; CQ_EndGameBossBattleScript q = { false, false, false }; CQ_SunnyvaleMasterData md = { 0 };
        CSwordVoid s(q); s.Game = &game; s.Me.id = 1; s.PMasterData = &md; game.hero.id = 7;
        g_n = 0; g_doneAfter = 1; g_doneCalls = 0;
        s.Main();
        static const int want[] = { 1, 30, 2, 20, 31 };
        ok = ok && g_n == 5 && std::memcmp(g_log, want, sizeof(want)) == 0;
    }
    {   // decision ready, hero near, answers "no" (0) then "yes" (1): the ending plays, then idle until terminating
        CGame game; CQ_EndGameBossBattleScript q = { true, false, false }; CQ_SunnyvaleMasterData md = { 0 };
        CSwordVoid s(q); s.Game = &game; s.Me.id = 1; s.PMasterData = &md; game.hero.id = 7;
        g_n = 0; g_doneAfter = 100; g_doneCalls = 0; g_near = true; g_valid = true;
        g_answers[0] = 0; g_answers[1] = 1; g_answerIdx = 0;
        s.Main();
        ok = ok && q.FinalDecisionMade && q.FinalCutscenePlayed && md.JackBossBattleResult == 2 && g_answerIdx == 2;
        // the cutscene's order: hero handle, release, start, movie, start movie, pause, macro, unpause, movie dtor, handle dtor
        static const int seq[] = { 10, 12, 24, 31, 13, 25, 26, 29, 27, 14, 11 };
        int k = 0;
        for (int i = 0; i < g_n && k < 11; ++i) if (g_log[i] == seq[k]) ++k;
        ok = ok && k == 11 && g_log[g_n - 1] == 4;   // the timer goes last
    }
    if (!ok) { std::printf("00d33280_TEST FAIL\n"); return 1; }
    std::printf("00d33280_TEST PASS\n");
    return 0;
}