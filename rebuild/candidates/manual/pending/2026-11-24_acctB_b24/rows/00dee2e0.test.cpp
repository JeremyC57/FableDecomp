// Behaviour test for 0x00DEE2E0 NScript::CQ_SecretPassageScript::CMotherTorture::Main: model plus the body verbatim from the row source,
// callees stubbed. Checks the alive predicate on Me, the opening frame and early exit, the unkillable / no-combo / quest-orb set-up, the
// not-found block (control taken with retries, the lever wait, the door opened, objective 06 at GraveyardCircle, green orb, MotherFound
// set, control released) and its skip when already found, the follow set-up, and the main loop: hero talk (LETS_GO only while alive),
// hero hit (melee or a non-heal special ability: ON_HIT), the heal-life spell ignored, and on an expired speech timer the
// HIT_BY_ENEMY / FIGHTING_COMPLIMENT (combo > 3, rand % 100) / USE_FLOURISH (rand % 200) / NOT_FAR_NOW (rand % 400) lines with the
// 15-second reset; every exit releases the handle, timer, movie, pause and thing copies.
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#ifndef _MSC_VER
#define _vsnprintf vsnprintf
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail++; printf("FAIL line %d: %s\n", __LINE__, #c); } } while (0)
static char g_log[16384];
static void Log(const char* fmt, ...) { char b[256]; va_list a; va_start(a, fmt); _vsnprintf(b, sizeof(b) - 1, fmt, a); b[sizeof(b) - 1] = 0; va_end(a); strcat(g_log, b); strcat(g_log, ";"); }
static int g_frames = 0, g_iatt = 0, g_termFrame = 1 << 30;
static int g_live = 0, g_movies = 0, g_pauses = 0, g_handles = 0, g_timers = 0;
class CCharString { public:
    char Text[128];
    CCharString(const char* t, long len = -1) { (void)len; strncpy(Text, t, 127); Text[127] = 0; }
    CCharString(const CCharString& o) { strcpy(Text, o.Text); }
};
enum EHeroAbility { HERO_ABILITY_HEAL_LIFE_SPELL = 14 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE = 0, GROUP_SELECT_RANDOM = 1, GROUP_SELECT_RANDOM_NO_REPEAT = 2 };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_HIGHEST = 4 };
enum EMusicSetType { MUSIC_SET_DANGER = 3 };
static int g_heroHitAt = -1, g_heroHitKind = 0;   // 1 melee, 2 heal-life spell, 3 another special ability
static int g_anyHitAt = -1, g_talkAt = -1, g_leverAt = 1 << 30;
static int g_rand[64], g_nrand = 0, g_irand = 0, g_hits = 0, g_flourish = 0;
static int TestRand() { int r = g_irand < g_nrand ? g_rand[g_irand] : 1; ++g_irand; return r; }
#define rand TestRand
class CScriptThing { public:
    int Id;
    explicit CScriptThing(int id) : Id(id) { ++g_live; }
    CScriptThing(const CScriptThing& o) : Id(o.Id) { ++g_live; }
    ~CScriptThing() { --g_live; }
    bool MsgIsHitBy(const CCharString& a) const { CHECK(Id == 7);
        if (a.Text[0] == 0) return g_frames == g_anyHitAt;
        CHECK(strcmp(a.Text, "SCRIPT_NAME_HERO") == 0); return g_frames == g_heroHitAt && g_heroHitKind == 1; }
    bool MsgIsHitByAnySpecialAbilityFrom(const CCharString& a) const { CHECK(strcmp(a.Text, "SCRIPT_NAME_HERO") == 0); return g_frames == g_heroHitAt && g_heroHitKind >= 2; }
    bool MsgIsHitBySpecialAbilityFrom(EHeroAbility ab, const CCharString& a) const { CHECK(ab == HERO_ABILITY_HEAL_LIFE_SPELL); CHECK(strcmp(a.Text, "SCRIPT_NAME_HERO") == 0); return g_frames == g_heroHitAt && g_heroHitKind == 2; }
    bool MsgIsTalkedToBy(const CCharString& a) const { CHECK(Id == 7); CHECK(strcmp(a.Text, "SCRIPT_NAME_HERO") == 0); return g_frames == g_talkAt; }
    bool MsgIsUsedBy(const CCharString& a) const { CHECK(Id == 40); CHECK(strcmp(a.Text, "SCRIPT_NAME_HERO") == 0); return g_frames >= g_leverAt; }
};
static long g_timer = 0;
class CTimer { public:
    CTimer() { ++g_timers; }
    ~CTimer() { --g_timers; }
    CTimer& operator=(long n) { g_timer = n; Log("timer %ld", n); return *this; }
    bool operator==(long n) const { return g_timer == n; }
};
static float g_health = 100.0f;
static int g_startFails = 0, g_speakFrames = 0;
class CScriptGameResourceObjectScriptedThingBase { public:
    bool Valid; int Busy;
    CScriptGameResourceObjectScriptedThingBase() : Valid(false), Busy(0) { ++g_handles; }
    ~CScriptGameResourceObjectScriptedThingBase() { --g_handles; }
    bool IsValid() const { return Valid; }
    void Release() { CHECK(Valid); Valid = false; Log("release"); }
    CScriptThing GetScriptThing() const { CHECK(Valid); return CScriptThing(7); }
    void Speak(const CScriptThing& to, const char* key, ETextGroupSelectionMethod m, bool a, bool b, bool c) { CHECK(to.Id == 1); CHECK(Valid); CHECK(!a && b && !c); CHECK(g_movies == 1 && g_pauses == 1); Log("say %s %d", key, (int)m); Busy = g_speakFrames; }
    bool IsPerformingScriptTask() { return Busy-- > 0; }
};
class TRetVoid { public: void Return() { } };
class CScriptGameResourceObjectMovieBase { public:
    CScriptGameResourceObjectMovieBase() { ++g_movies; }
    ~CScriptGameResourceObjectMovieBase() { --g_movies; }
};
class CGameScriptInterface { public:
    void NewScriptFrame() { ++g_frames; if (g_timer > 0) --g_timer; }
    float GetHealth(const CScriptThing& t) { CHECK(t.Id == 7); return g_health; }
    CScriptThing GetHero() { return CScriptThing(1); }
    bool StartScriptingEntity(const CScriptThing& t, CScriptGameResourceObjectScriptedThingBase& h, EScriptAIPriority p) { CHECK(t.Id == 7 && p == SCRIPT_AI_PRIORITY_HIGHEST); CHECK(!h.Valid); if (g_startFails > 0) { g_startFails--; return false; } h.Valid = true; Log("control"); return true; }
    void StartMovieSequence(const CCharString& name, CScriptGameResourceObjectMovieBase&) { CHECK(name.Text[0] == 0); CHECK(g_movies == 1); Log("movie"); }
    void EntitySetAsKillable(const CScriptThing& t, bool a, bool b) { CHECK(t.Id == 7 && !a && !b); Log("unkillable"); }
    void EntitySetAsToAddToComboMultiplierWhenHit(const CScriptThing& t, bool a) { CHECK(t.Id == 7 && !a); Log("nocombo"); }
    void MiniMapAddMarker(const CScriptThing& t, const CCharString& s) { CHECK(t.Id == 7); Log("orb %s", s.Text); }
    void MiniMapSetMarkerGraphic(const CScriptThing& t, const CCharString& s) { CHECK(t.Id == 7); Log("orbgfx %s", s.Text); }
    CScriptThing GetThingWithScriptName(const CCharString& n) { if (strcmp(n.Text, "MotherCellLever") == 0) return CScriptThing(40); CHECK(strcmp(n.Text, "MotherCellDoor") == 0); return CScriptThing(41); }
    void EntitySetTargetable(const CScriptThing& d, bool b) { CHECK(d.Id == 41 && !b); Log("untargetable"); }
    void OpenDoor(const CScriptThing& d) { CHECK(d.Id == 41); Log("open door"); }
    CCharString GetActiveQuestName() { return CCharString("Q_PRISON"); }
    void SetQuestCardObjective(const CCharString& q, const CCharString& o, const CCharString& r1, const CCharString& r2) { CHECK(strcmp(q.Text, "Q_PRISON") == 0); CHECK(r2.Text[0] == 0); Log("objective %s %s", o.Text, r1.Text); }
    void EntitySetDeedReactionsEnabled(const CScriptThing& t, bool b) { CHECK(t.Id == 7 && !b); Log("nodeeds"); }
    void EntitySetOpinionReactionsEnabled(const CScriptThing& t, bool b) { CHECK(t.Id == 7 && !b); Log("noopinion"); }
    void EntitySetWillBeUsingNarrator(const CScriptThing& t, const CCharString& s) { CHECK(t.Id == 7); Log("narrator %s", s.Text); }
    void EntityFollowThing(const CScriptThing& t, const CScriptThing& h, float d, bool b) { CHECK(t.Id == 7 && h.Id == 1 && d == 3.0f && b); Log("follow"); }
    void SetEntityAsRegionFollowing(const CScriptThing& h, const CScriptThing& t, bool b) { CHECK(t.Id == 7 && h.Id == 1 && b); Log("regionfollow"); }
    void OverrideMusic(EMusicSetType m, bool a, bool b) { CHECK(m == MUSIC_SET_DANGER && !a && !b); Log("music"); }
    long AddNewConversation(const CScriptThing& t, bool a, bool b) { CHECK(t.Id == 7 && !a && !b); return 55; }
    void AddLineToConversation(long i, const CCharString& s, bool a, const CScriptThing& sp, const CScriptThing& li) { CHECK(i == 55 && !a && sp.Id == 7 && li.Id == 1); Log("line %s", s.Text); }
    long GetPlayerCreatureCombatMultiplierRunningNumHits() { return g_hits; }
    bool IsPlayerCreatureFlourishEnabled() { return g_flourish != 0; }
};
class CWideScreenMagicPauseEntities { public:
    explicit CWideScreenMagicPauseEntities(CGameScriptInterface&) { ++g_pauses; }
    ~CWideScreenMagicPauseEntities() { --g_pauses; }
};
namespace NScript {
struct CIsThingAlive { CScriptThing Thing; CIsThingAlive(const CScriptThing& t) : Thing(t) { } };
class CQ_SecretPassageScript { public:
    bool MotherFound;
    CQ_SecretPassageScript() : MotherFound(false) { }
    class CMotherTorture { public:
        CGameScriptInterface* Game;
        CScriptThing Me;
        CQ_SecretPassageScript* ParentClass;
        int PredicateId;
        CMotherTorture(CQ_SecretPassageScript* p, CGameScriptInterface* g) : Game(g), Me(7), ParentClass(p), PredicateId(0) { }
        void SetPredicate(const CIsThingAlive& p) { PredicateId = p.Thing.Id; }
        bool IsActiveThreadTerminating() { ++g_iatt; return g_frames >= g_termFrame; }
        void Main();
    };
};
}
void NScript::CQ_SecretPassageScript::CMotherTorture::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    Game->EntitySetAsKillable(Me,false,false);
    Game->EntitySetAsToAddToComboMultiplierWhenHit(Me,false);
    Game->MiniMapAddMarker(Me,"HUD_ORB_QUEST_CORE");
    if (!ParentClass->MotherFound) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (seh_me.IsValid()) seh_me.Release();
        while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        CScriptThing MotherLever=Game->GetThingWithScriptName("MotherCellLever");
        CScriptThing MotherCellDoor=Game->GetThingWithScriptName("MotherCellDoor");
        Game->EntitySetTargetable(MotherCellDoor,false);
        while (!MotherLever.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->OpenDoor(MotherCellDoor);
        Game->SetQuestCardObjective(Game->GetActiveQuestName(),"TEXT_QUEST_PRISON_ESCAPE_OBJECTIVE_06","GraveyardCircle","");
        Game->MiniMapSetMarkerGraphic(Me,"HUD_ORB_GREEN_SMALL");
        ParentClass->MotherFound=true;
        if (seh_me.IsValid()) seh_me.Release();
    }
    Game->EntitySetDeedReactionsEnabled(Me,false);
    Game->EntitySetOpinionReactionsEnabled(Me,false);
    Game->EntitySetWillBeUsingNarrator(Me,"SCARLET_ROBE");
    Game->EntityFollowThing(Me,Game->GetHero(),3.0f,true);
    Game->SetEntityAsRegionFollowing(Game->GetHero(),Me,true);
    Game->OverrideMusic(MUSIC_SET_DANGER,false,false);
    CTimer MotherSpeechTimer;
    if (false) {} else for (;; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                seh_me.Speak(Game->GetHero(),"TEXT_QST_055_MOTHER_LETS_GO",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (seh_me.IsValid()) seh_me.Release();
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                seh_me.Speak(Game->GetHero(),"TEXT_QST_055_MOTHER_ON_HIT",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (seh_me.IsValid()) seh_me.Release();
        } else if (MotherSpeechTimer==0) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Me.MsgIsHitBy("")) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long HLStyleConversationIndex=Game->AddNewConversation(Me,false,false);
                Game->AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_055_MOTHER_HIT_BY_ENEMY",false,Me,Game->GetHero());
                MotherSpeechTimer=15;
            }
            else if (Game->GetPlayerCreatureCombatMultiplierRunningNumHits()>3 && rand()%100==0) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long HLStyleConversationIndex=Game->AddNewConversation(Me,false,false);
                Game->AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_055_MOTHER_FIGHTING_COMPLIMENT",false,Me,Game->GetHero());
                MotherSpeechTimer=15;
            }
            else if (Game->IsPlayerCreatureFlourishEnabled() && rand()%200==0) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long HLStyleConversationIndex=Game->AddNewConversation(Me,false,false);
                Game->AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_055_MOTHER_USE_FLOURISH",false,Me,Game->GetHero());
                MotherSpeechTimer=15;
            }
            else if (rand()%400==0) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long HLStyleConversationIndex=Game->AddNewConversation(Me,false,false);
                Game->AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_055_MOTHER_NOT_FAR_NOW",false,Me,Game->GetHero());
                MotherSpeechTimer=15;
            }
        }
    }
}

static void Reset() {
    g_log[0] = 0; g_frames = 0; g_iatt = 0; g_termFrame = 1 << 30; g_heroHitAt = -1; g_heroHitKind = 0; g_anyHitAt = -1; g_talkAt = -1;
    g_leverAt = 1 << 30; g_nrand = 0; g_irand = 0; g_hits = 0; g_flourish = 0; g_timer = 0; g_health = 100.0f; g_startFails = 0; g_speakFrames = 0;
}
static bool Has(const char* s) { return strstr(g_log, s) != 0; }
static bool Before(const char* a, const char* b) { const char* pa = strstr(g_log, a); const char* pb = strstr(g_log, b); return pa && pb && pa < pb; }
static void Run(bool found) {
    NScript::CQ_SecretPassageScript parent; parent.MotherFound = found;
    CGameScriptInterface game;
    {
        NScript::CQ_SecretPassageScript::CMotherTorture m(&parent, &game);
        m.Main();
        CHECK(m.PredicateId == 7);
        CHECK(g_live == 1);
    }
    CHECK(parent.MotherFound || g_frames < 2 || !Has("open door"));
    CHECK(g_live == 0 && g_movies == 0 && g_pauses == 0 && g_handles == 0 && g_timers == 0);
    if (Has("open door")) CHECK(parent.MotherFound);
#ifdef TRACE
    printf("frames %d rand %d log %s\n", g_frames, g_irand, g_log);
#endif
}
int main()
{
    // A: not found yet -- control with one retry, lever on frame 3, then talk (frame 4), hero melee hit (6), enemy hit (8)
    Reset(); g_startFails = 1; g_leverAt = 3; g_talkAt = 4; g_heroHitAt = 6; g_heroHitKind = 1; g_anyHitAt = 8; g_speakFrames = 1; g_termFrame = 12;
    g_rand[0] = 1; g_rand[1] = 1; g_rand[2] = 1; g_rand[3] = 1; g_nrand = 4;
    Run(false);
    CHECK(Before("unkillable;nocombo;orb HUD_ORB_QUEST_CORE", "control"));
    CHECK(Before("control;untargetable;open door;objective TEXT_QUEST_PRISON_ESCAPE_OBJECTIVE_06 GraveyardCircle;orbgfx HUD_ORB_GREEN_SMALL;release", "nodeeds"));
    CHECK(Has("nodeeds;noopinion;narrator SCARLET_ROBE;follow;regionfollow;music;"));
    CHECK(Before("movie;control;say TEXT_QST_055_MOTHER_LETS_GO 2;release", "movie;control;say TEXT_QST_055_MOTHER_ON_HIT 2;release"));
    CHECK(Has("line TEXT_QST_055_MOTHER_HIT_BY_ENEMY;timer 15"));
    CHECK(g_irand == 2);   // the timer branch ran (one rand each) on frames 3 and 5 only: talk spans 4-5, the hit 6-7, the enemy line at 8 resets it
    CHECK(!Has("FIGHTING") && !Has("FLOURISH") && !Has("NOT_FAR"));
    // B: already found -- no control block; heal-life spell ignored, other special ability answered; combo > 3 compliment
    Reset(); g_talkAt = -1; g_heroHitAt = 2; g_heroHitKind = 2; g_hits = 5; g_rand[0] = 7; g_rand[1] = 1; g_rand[2] = 100; g_nrand = 3; g_termFrame = 4;
    Run(true);
    CHECK(!Has("control;untargetable") && !Has("open door") && !Has("ON_HIT"));
    CHECK(Has("line TEXT_QST_055_MOTHER_FIGHTING_COMPLIMENT;timer 15") && g_irand == 3);   // frame 1: % 100 and % 400 miss; frame 2 (heal hit ignored): hit
    Reset(); g_heroHitAt = 2; g_heroHitKind = 3; g_termFrame = 4; g_rand[0] = 3; g_nrand = 1; g_hits = 4;
    Run(true);
    CHECK(Has("say TEXT_QST_055_MOTHER_ON_HIT 2"));
    // C: flourish line, then the 15-frame timer holds off further lines; then not-far-now
    Reset(); g_flourish = 1; g_rand[0] = 200; g_nrand = 1; g_termFrame = 10;
    Run(true);
    CHECK(Has("line TEXT_QST_055_MOTHER_USE_FLOURISH;timer 15") && g_irand == 1 && !Has("NOT_FAR"));
    Reset(); g_rand[0] = 5; g_rand[1] = 800; g_nrand = 2; g_termFrame = 6;
    Run(true);
    CHECK(Has("line TEXT_QST_055_MOTHER_NOT_FAR_NOW;timer 15") && g_irand == 2);
    // D: dead mother -- talk takes control and releases it without a line
    Reset(); g_talkAt = 2; g_health = 0.0f; g_termFrame = 4; g_rand[0] = g_rand[1] = g_rand[2] = 1; g_nrand = 3;
    Run(true);
    CHECK(Has("movie;control;release") && !Has("say"));
    // E: terminating on the first frame -- nothing set up
    Reset(); g_termFrame = 1;
    Run(false);
    CHECK(g_log[0] == 0 && g_iatt == 1);
    // F: terminating while waiting for the lever -- door stays shut, flag clear
    Reset(); g_termFrame = 3;
    Run(false);
    CHECK(Has("control;untargetable") && !Has("open door"));
    if (g_fail) { printf("00dee2e0_TEST FAIL\n"); return 1; }
    printf("00dee2e0_TEST PASS\n");
    return 0;
}
