// Behaviour test for 0x00D67890 NScript::CQ_GuildTrainingWoodsWillScript::Main: model plus the body verbatim from the row source, callees
// stubbed. Checks InitialiseVariables, the WillWhisper binding, the GuildWoods wait and early exits, objective / threads / quest card, the
// take-over of Whisper, the bandits (resize to the bandit count, one handle each) and the hero, the intro cutscene map (HERO, BAN1..3,
// WHISPER) under a movie + pause, the bandits set on the hero at 15 health, the main loop (level reload wait, bandit re-collection, all
// dead -> BanditsAlive false, low health -> Whisper's line, Xbox / PC strafe hint, WhisperAnimate, heal), the outro and MissionSucceeded.
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <vector>
#include <map>
#ifndef _MSC_VER
#define _vsnprintf vsnprintf
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail++; printf("FAIL line %d: %s\n", __LINE__, #c); } } while (0)
static char g_log[8192];
static void Log(const char* fmt, ...) { char b[256]; va_list a; va_start(a, fmt); _vsnprintf(b, sizeof(b) - 1, fmt, a); b[sizeof(b) - 1] = 0; va_end(a); strcat(g_log, b); strcat(g_log, ";"); }
static int g_frames = 0, g_iatt = 0, g_termAt = 1 << 30, g_termFrame = 1 << 30;
static int g_handles = 0, g_movies = 0, g_pauses = 0;
static bool Term() { ++g_iatt; return g_iatt >= g_termAt || g_frames >= g_termFrame; }
class CCharString { public:
    char Text[128];
    CCharString(const char* t = "", long len = -1) { (void)len; strncpy(Text, t, 127); Text[127] = 0; }
    bool operator<(const CCharString& o) const { return strcmp(Text, o.Text) < 0; }
};
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_HIGHEST = 4 };
static float g_health[8];
static bool g_killOnUnload[8];
class CScriptThing { public:
    int Id;
    CScriptThing(int id = 0) : Id(id) { }
    void SetToKillOnLevelUnload(bool b) { g_killOnUnload[Id] = b; }
};
class CScriptGameResourceObjectScriptedThingBase { public:
    int Who;
    CScriptGameResourceObjectScriptedThingBase() : Who(0) { ++g_handles; }
    CScriptGameResourceObjectScriptedThingBase(const CScriptGameResourceObjectScriptedThingBase& o) : Who(o.Who) { ++g_handles; }
    CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& o) { Who = o.Who; return *this; }
    ~CScriptGameResourceObjectScriptedThingBase() { --g_handles; }
    bool IsValid() const { return Who != 0; }
    void Release() { Log("release %d", Who); Who = 0; }
};
class CScriptGameResourceObjectMovieBase { public:
    CScriptGameResourceObjectMovieBase() { ++g_movies; }
    ~CScriptGameResourceObjectMovieBase() { --g_movies; }
};
class TRetVoid { public: void Return() { } };
struct CScriptDef { float GUI_MinHealth; };
static CScriptDef g_def = { 20.0f };
static CScriptDef* pGlobalScriptDef = &g_def;
static int g_loadedFrom = 0, g_unloadAt = -1, g_unloadUntil = -1, g_banditsDieAt = 1 << 30, g_lowAt = -1, g_startFails = 0, g_clickWait = 0;
static bool g_xbox = false;
static int g_nBandits = 3;
class CGameScriptInterface { public:
    void PostAddScriptedEntities() { Log("game post add"); }
    bool IsLevelLoaded(const CCharString& n) { CHECK(strcmp(n.Text, "GuildWoods") == 0); return g_frames >= g_loadedFrom && !(g_frames >= g_unloadAt && g_frames < g_unloadUntil); }
    void NewScriptFrame() { ++g_frames; }
    void SetQuestCardObjective(const CCharString& q, const CCharString& o, const CCharString& a, const CCharString& b) { CHECK(a.Text[0] == 0 && b.Text[0] == 0); Log("objective %s %s", q.Text, o.Text); }
    void GiveHeroQuestCardDirectly(const CCharString& c, const CCharString& q, bool b) { CHECK(!b && strcmp(q.Text, "Q_GuildTrainingWoodsWill") == 0); Log("card %s", c.Text); }
    CScriptThing GetThingWithScriptName(const CCharString& n) { CHECK(strcmp(n.Text, "WillWhisper") == 0); return CScriptThing(1); }
    CScriptThing GetHero() { return CScriptThing(2); }
    bool StartScriptingEntity(const CScriptThing& t, CScriptGameResourceObjectScriptedThingBase& h, EScriptAIPriority p) { CHECK(p == SCRIPT_AI_PRIORITY_HIGHEST); CHECK(!h.IsValid()); if (t.Id == 1 && g_startFails > 0) { g_startFails--; return false; } h.Who = t.Id; Log("start %d", t.Id); return true; }
    void GetAllThingsWithScriptName(const CCharString& n, std::vector<CScriptThing>& v) { CHECK(strcmp(n.Text, "WillBandit") == 0); CHECK(v.empty()); int k = g_frames >= g_banditsDieAt ? 0 : g_nBandits; for (int i = 0; i < k; i++) v.push_back(CScriptThing(4 + i)); }
    void StartMovieSequence(const CCharString& n, CScriptGameResourceObjectMovieBase&) { CHECK(n.Text[0] == 0 && g_movies > 0); Log("movie"); }
    void FixMovieSequenceCamera(bool b) { Log("fix %d", (int)b); }
    void GiveThingBestEnemyTarget(const CScriptThing& t, const CScriptThing& e) { CHECK(e.Id == 2); Log("target %d", t.Id); }
    float GetHealth(const CScriptThing& t) { return g_health[t.Id]; }
    void ModifyThingHealth(const CScriptThing& t, float d, bool b) { CHECK(!b); g_health[t.Id] += d; }
    long AddNewConversation(const CScriptThing& t, bool a, bool b) { CHECK(t.Id == 1 && !a && !b); return 77; }
    void AddPersonToConversation(long c, const CScriptThing& t) { CHECK(c == 77 && t.Id == 2); }
    void AddLineToConversation(long c, const CCharString& l, bool b, const CScriptThing& s, const CScriptThing& to) { CHECK(c == 77 && !b && s.Id == 1 && to.Id == 2); Log("line %s", l.Text); }
    bool IsXbox() { return g_xbox; }
    void DisplayGameInfo(const CCharString& s) { Log("info %s", s.Text); }
    bool MsgIsGameInfoClickedPast() { return g_clickWait-- <= 0; }
    void ChangeHeroHealthBy(float d, bool a, bool b) { CHECK(a && !b); g_health[2] += d; Log("heal"); }
};
class CWideScreenMagicPauseEntities { public:
    explicit CWideScreenMagicPauseEntities(CGameScriptInterface&) { ++g_pauses; }
    ~CWideScreenMagicPauseEntities() { --g_pauses; }
};
static void RunCutsceneMacro(const CCharString& n, std::map<CCharString, CScriptGameResourceObjectScriptedThingBase>& m, int a, int b, bool c, bool d)
{
    CHECK(a == 0 && b == 0 && !c && d); CHECK(g_movies == 1 && g_pauses == 1);
    char k[256]; k[0] = 0;
    for (std::map<CCharString, CScriptGameResourceObjectScriptedThingBase>::iterator i = m.begin(); i != m.end(); ++i) { char e[32]; sprintf(e, " %s=%d", i->first.Text, i->second.Who); strcat(k, e); }
    Log("cutscene %s%s", n.Text, k);
}
template <class T> struct CEntityScriptBinding { CCharString Name; T* P; int N; template <class F> CEntityScriptBinding(const char* n, T* p, F, int k) : Name(n), P(p), N(k) { } };
template <class T> struct CSpawnedFunc { CCharString Name; T* P; void (T::*F)(); CSpawnedFunc(const char* n, int id, T* p, void (T::*f)()) : Name(n), P(p), F(f) { CHECK(id == 0); } };
namespace NScript {
class CQ_GuildTrainingWoodsWillScript { public:
    CGameScriptInterface* Game;
    bool MissionSucceeded, MissionFailed, MissionOver, WhisperAnimate, BanditsAlive;
    int Bindings, Spawned;
    CQ_GuildTrainingWoodsWillScript() : MissionSucceeded(true), MissionFailed(true), MissionOver(true), WhisperAnimate(true), BanditsAlive(false), Bindings(0), Spawned(0) { }
    void InitialiseVariables() { MissionSucceeded = false; MissionFailed = false; MissionOver = false; WhisperAnimate = false; BanditsAlive = true; }
    struct CWillWhisper { static int Alloc() { return 0; } };
    void AddEntityScriptBinding(CEntityScriptBinding<CQ_GuildTrainingWoodsWillScript>* b) { CHECK(strcmp(b->Name.Text, "WillWhisper") == 0 && b->P == this && b->N == 1); Bindings++; delete b; }
    void PostAddScriptedEntities() { Log("post add"); }
    void AddSpawnedFunction(CSpawnedFunc<CQ_GuildTrainingWoodsWillScript>* f, const CCharString& r) { CHECK(r.Text[0] == 0 && f->P == this); Log("spawn %s", f->Name.Text); Spawned++; delete f; }
    void WatchForTermination() { }
    void DoMission() { }
    bool IsActiveThreadTerminating() { return Term(); }
    void Main();
};
}
void NScript::CQ_GuildTrainingWoodsWillScript::Main()
{
    TRetVoid _Fail_Val;
    InitialiseVariables();
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_GuildTrainingWoodsWillScript>("WillWhisper", this, &CWillWhisper::Alloc, 1));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    while (!Game->IsLevelLoaded("GuildWoods")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    Game->SetQuestCardObjective("Q_GuildTrainingWoodsWill", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_06_OPTION_02", "", "");
    AddSpawnedFunction(new CSpawnedFunc<CQ_GuildTrainingWoodsWillScript>("WatchForTermination", 0, this, &CQ_GuildTrainingWoodsWillScript::WatchForTermination), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_GuildTrainingWoodsWillScript>("DoMission", 0, this, &CQ_GuildTrainingWoodsWillScript::DoMission), CCharString(""));
    Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_TRAINING_KILL_BANDITS", "Q_GuildTrainingWoodsWill", false);
    CScriptThing Whisper = Game->GetThingWithScriptName("WillWhisper");
    CScriptGameResourceObjectScriptedThingBase seh_Whisper;
    if (seh_Whisper.IsValid())
        seh_Whisper.Release();
    while (!Game->StartScriptingEntity(Whisper, seh_Whisper, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    std::vector<CScriptThing> AllBandits;
    Game->GetAllThingsWithScriptName("WillBandit", AllBandits);
    {
        std::vector<CScriptGameResourceObjectScriptedThingBase> BanditHandles;
        BanditHandles.resize(AllBandits.size());
        if (false) {} else for (long i = 0; i < AllBandits.size(); i++)
            Game->StartScriptingEntity(AllBandits[i], BanditHandles[i], SCRIPT_AI_PRIORITY_HIGHEST);
        CScriptGameResourceObjectScriptedThingBase seh_Hero;
        if (seh_Hero.IsValid())
            seh_Hero.Release();
        while (!Game->StartScriptingEntity(Game->GetHero(), seh_Hero, SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
        mymap["HERO"] = seh_Hero;
        mymap["BAN1"] = BanditHandles[0];
        mymap["BAN2"] = BanditHandles[1];
        mymap["BAN3"] = BanditHandles[2];
        mymap["WHISPER"] = seh_Whisper;
        CScriptGameResourceObjectMovieBase speech_movie;
        Game->StartMovieSequence("", speech_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
        Game->FixMovieSequenceCamera(true);
        RunCutsceneMacro("CS_GUILD_WILL_WOODS_INTRO", mymap, 0, 0, false, true);
        Game->FixMovieSequenceCamera(false);
    }
    if (false) {} else for (long i = 0; i < AllBandits.size(); i++) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        Game->GiveThingBestEnemyTarget(AllBandits[i], Game->GetHero());
        Game->ModifyThingHealth(AllBandits[i], 15.0f - Game->GetHealth(AllBandits[i]), false);
        AllBandits[i].SetToKillOnLevelUnload(false);
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    if (seh_Whisper.IsValid())
        seh_Whisper.Release();
    while (BanditsAlive) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        if (!Game->IsLevelLoaded("GuildWoods")) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            while (!Game->IsLevelLoaded("GuildWoods")) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
        }
        while (!AllBandits.empty())
            AllBandits.pop_back();
        Game->GetAllThingsWithScriptName("WillBandit", AllBandits);
        if (AllBandits.size() == 0) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            BanditsAlive = false;
        }
        if (Game->GetHealth(Game->GetHero()) < pGlobalScriptDef->GUI_MinHealth) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            long HLStyleConversationIndex = Game->AddNewConversation(Whisper, false, false);
            Game->AddPersonToConversation(HLStyleConversationIndex, Game->GetHero());
            Game->AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_028_TEEN_WHISPER_SKILL_WOODS_HEALTH", false, Whisper, Game->GetHero());
            if (Game->IsXbox()) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                Game->DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_STRAFE_HELP");
                while (!Game->MsgIsGameInfoClickedPast()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
            } else {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                Game->DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_STRAFE_HELP_PC");
                while (!Game->MsgIsGameInfoClickedPast()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
            }
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            WhisperAnimate = true;
            Game->ChangeHeroHealthBy(1000.0f, true, false);
        }
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    if (seh_Whisper.IsValid())
        seh_Whisper.Release();
    while (!Game->StartScriptingEntity(Whisper, seh_Whisper, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    {
        CScriptGameResourceObjectScriptedThingBase seh_Hero;
        if (seh_Hero.IsValid())
            seh_Hero.Release();
        while (!Game->StartScriptingEntity(Game->GetHero(), seh_Hero, SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
        mymap["HERO"] = seh_Hero;
        mymap["WHISPER"] = seh_Whisper;
        CScriptGameResourceObjectMovieBase speech_movie;
        Game->StartMovieSequence("", speech_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
        Game->FixMovieSequenceCamera(true);
        RunCutsceneMacro("CS_GUILD_WILL_WOODS_OUTRO", mymap, 0, 0, false, true);
        Game->FixMovieSequenceCamera(false);
    }
    MissionSucceeded = true;
}

static void Reset()
{
    g_log[0] = 0; g_frames = 0; g_iatt = 0; g_termAt = 1 << 30; g_termFrame = 1 << 30;
    g_loadedFrom = 0; g_unloadAt = -1; g_unloadUntil = -1; g_banditsDieAt = 1 << 30; g_lowAt = -1; g_startFails = 0; g_clickWait = 0; g_xbox = false; g_nBandits = 3;
    for (int i = 0; i < 8; i++) { g_health[i] = 100.0f; g_killOnUnload[i] = true; }
}
static void Run(NScript::CQ_GuildTrainingWoodsWillScript& s)
{
    CGameScriptInterface game; s.Game = &game; s.Main();
    CHECK(g_handles == 0 && g_movies == 0 && g_pauses == 0);
}
static bool Has(const char* t) { return strstr(g_log, t) != 0; }
#define INTRO "post add;game post add;objective Q_GuildTrainingWoodsWill TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_06_OPTION_02;spawn WatchForTermination;spawn DoMission;card OBJECT_QUEST_CARD_TRAINING_KILL_BANDITS;start 1;start 4;start 5;start 6;start 2;movie;fix 1;cutscene CS_GUILD_WILL_WOODS_INTRO BAN1=4 BAN2=5 BAN3=6 HERO=2 WHISPER=1;fix 0;target 4;target 5;target 6;release 1;"
typedef NScript::CQ_GuildTrainingWoodsWillScript Q;
int main()
{
    // 1. the level never loads: terminate in the wait; flags reset, binding added, nothing else
    { Reset(); Q q; g_loadedFrom = 100; g_termFrame = 3; Run(q);
      CHECK(!q.MissionSucceeded && !q.MissionFailed && !q.MissionOver && !q.WhisperAnimate && q.BanditsAlive && q.Bindings == 1);
      CHECK(strcmp(g_log, "post add;game post add;") == 0 && q.Spawned == 0); }
    // 2. full run: intro, bandits at 15 health and kept on unload off, die at frame 6, outro, success
    { Reset(); Q q; g_loadedFrom = 2; g_startFails = 1; g_banditsDieAt = 6; g_termFrame = 30; Run(q);
      CHECK(strncmp(g_log, INTRO, strlen(INTRO)) == 0);
      CHECK(Has("start 1;start 2;movie;fix 1;cutscene CS_GUILD_WILL_WOODS_OUTRO HERO=2 WHISPER=1;fix 0;"));
      CHECK(g_health[4] == 15.0f && g_health[6] == 15.0f && !g_killOnUnload[5]); CHECK(!q.BanditsAlive && q.MissionSucceeded && q.Spawned == 2); }
    // 3. hero low on health (PC): Whisper's line, the PC strafe hint after two frames, WhisperAnimate, heal
    { Reset(); Q q; g_health[2] = 10.0f; g_clickWait = 2; g_banditsDieAt = 8; g_termFrame = 40; Run(q);
      CHECK(Has("line TEXT_QST_028_TEEN_WHISPER_SKILL_WOODS_HEALTH;info TEXT_QST_028_ONSCREENHELP_STRAFE_HELP_PC;heal;")); CHECK(q.WhisperAnimate && g_health[2] == 1010.0f && q.MissionSucceeded); }
    // 4. Xbox hint
    { Reset(); Q q; g_xbox = true; g_health[2] = 10.0f; g_banditsDieAt = 8; g_termFrame = 40; Run(q); CHECK(Has("info TEXT_QST_028_ONSCREENHELP_STRAFE_HELP;heal;")); }
    // 5. level unloaded in the main loop: wait for the reload, then go on
    { Reset(); Q q; g_unloadAt = 3; g_unloadUntil = 7; g_banditsDieAt = 9; g_termFrame = 40; Run(q); CHECK(q.MissionSucceeded && g_frames >= 9); }
    { Reset(); Q q; g_unloadAt = 3; g_unloadUntil = 100; g_termFrame = 10; Run(q); CHECK(!q.MissionSucceeded && q.BanditsAlive && Has("release 1;") && !Has("OUTRO")); }
    // 6. terminating in the main loop, and at the outro take-over
    { Reset(); Q q; g_termFrame = 5; Run(q); CHECK(!q.MissionSucceeded && q.BanditsAlive); }
    { Reset(); Q q; g_banditsDieAt = 3; g_termAt = 13; Run(q); CHECK(!q.MissionSucceeded); }
    if (g_fail) printf("00d67890_TEST FAIL %d\n", g_fail); else printf("00d67890_TEST PASS\n");
    return g_fail ? 1 : 0;
}
