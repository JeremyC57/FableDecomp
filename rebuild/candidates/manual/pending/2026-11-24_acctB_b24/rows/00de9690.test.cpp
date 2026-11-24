// Behaviour test for 0x00DE9690 NScript::CQ_RansomVictimScript::CRansomHelper::Main: model plus the body verbatim from the row source,
// callees stubbed. Checks the alive predicate on Me, the opening frame and early exit, the hero faction, the wait for the hero (or the
// helper start) with the hit test (melee / non-heal special ability / already attacked -> bandit forever), the start cutscene (guard
// alive: EXTERIOR_1 and the wait-outside flag; dead: EXTERIOR_2; door opened and persistent), the wait for RansomHelperSpoken, the main
// loop (hit by anyone: release, 8-second return timer, walk home when it runs out, home reached; talk: rescued / yet-to-pay lines with
// the data string, only when alive and not walking home), the hero-attack break to the bandit tail, and that every exit releases the
// scripted-thing handle, timer, movie, pause and copies.
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#ifndef _MSC_VER
#define _vsnprintf vsnprintf
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail++; printf("FAIL line %d: %s\n", __LINE__, #c); } } while (0)

static char g_log[8192];
static void Log(const char* fmt, ...) { char b[256]; va_list a; va_start(a, fmt); _vsnprintf(b, sizeof(b) - 1, fmt, a); b[sizeof(b) - 1] = 0; va_end(a); strcat(g_log, b); strcat(g_log, ";"); }
static int g_frames = 0, g_iatt = 0, g_termAt = 1 << 30, g_termFrame = 1 << 30;
static int g_live = 0, g_movies = 0, g_pauses = 0, g_handles = 0, g_timers = 0;
static bool Term() { ++g_iatt; return g_iatt >= g_termAt || g_frames >= g_termFrame; }

class CCharString { public:
    char Text[128];
    CCharString() { Text[0] = 0; }
    CCharString(const char* t, long len = -1) { (void)len; strncpy(Text, t, 127); Text[127] = 0; }
    CCharString(const CCharString& o) { strcpy(Text, o.Text); }
    operator const char*() const { return Text; }
};
CCharString operator+(const CCharString& l, const CCharString& r) { CCharString s(l); strcat(s.Text, r.Text); return s; }
CCharString operator+(const char* l, const CCharString& r) { CCharString s(l); strcat(s.Text, r.Text); return s; }
CCharString operator+(const CCharString& l, const char* r) { CCharString s(l); strcat(s.Text, r); return s; }

enum EHeroAbility { HERO_ABILITY_HEAL_LIFE_SPELL = 14 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE = 0, GROUP_SELECT_RANDOM = 1, GROUP_SELECT_RANDOM_NO_REPEAT = 2 };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_HIGHEST = 4 };
enum EScriptEntityMoveType { MOVE_WALK = 1 };
struct C3DVector { float X, Y, Z; };

static int g_heroHitAt = -1, g_heroHitKind = 0;   // 1 melee, 2 heal-life spell, 3 another special ability
static int g_anyHitAt = -1, g_talkAt = -1, g_talkAt2 = -1;
static bool g_guardAlive = true;
static int g_nearAt = 1 << 30;                    // frame from which the hero is within 10
static int g_homeAt = 1 << 30;                    // frame from which Me is within 2 of home
class CScriptThing { public:
    int Id;
    explicit CScriptThing(int id) : Id(id) { ++g_live; }
    CScriptThing(const CScriptThing& o) : Id(o.Id) { ++g_live; }
    ~CScriptThing() { --g_live; }
    bool MsgIsHitBy(const CCharString& a) const {
        if (a.Text[0] == 0) return g_frames == g_anyHitAt;
        CHECK(strcmp(a.Text, "SCRIPT_NAME_HERO") == 0); return g_frames == g_heroHitAt && g_heroHitKind == 1; }
    bool MsgIsHitByAnySpecialAbilityFrom(const CCharString& a) const { CHECK(strcmp(a.Text, "SCRIPT_NAME_HERO") == 0); return g_frames == g_heroHitAt && g_heroHitKind >= 2; }
    bool MsgIsHitBySpecialAbilityFrom(EHeroAbility ab, const CCharString& a) const { CHECK(ab == HERO_ABILITY_HEAL_LIFE_SPELL); CHECK(strcmp(a.Text, "SCRIPT_NAME_HERO") == 0); return g_frames == g_heroHitAt && g_heroHitKind == 2; }
    bool MsgIsTalkedToBy(const CCharString& a) const { CHECK(strcmp(a.Text, "SCRIPT_NAME_HERO") == 0); return g_frames == g_talkAt || g_frames == g_talkAt2; }
    bool IsAlive() const { CHECK(Id == 20); return g_guardAlive; }
    C3DVector GetHomePos() const { CHECK(Id == 7); C3DVector v = { 1.0f, 2.0f, 3.0f }; return v; }
    CCharString GetDataString() const { CHECK(Id == 7); return CCharString("HELPER2"); }
};
bool IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float d) { CHECK(a.Id == 7 && b.Id == 1 && d == 10.0f); return g_frames >= g_nearAt; }
bool IsDistanceFromThingToPositionUnder(const CScriptThing& a, const C3DVector& p, float d) { CHECK(a.Id == 7 && p.X == 1.0f && p.Z == 3.0f && d == 2.0f); return g_frames >= g_homeAt; }
static long g_timer = 0;
class CTimer { public:
    CTimer() { ++g_timers; }
    ~CTimer() { --g_timers; }
    CTimer& operator=(long n) { g_timer = n; Log("timer %ld", n); return *this; }
    bool operator==(long n) const { return g_timer == n; }
};
static float g_health = 100.0f;
static int g_startFails = 0, g_speakFrames = 0, g_moveBusy = 0;
class CScriptGameResourceObjectScriptedThingBase { public:
    bool Valid; int Busy;
    CScriptGameResourceObjectScriptedThingBase() : Valid(false), Busy(0) { ++g_handles; }
    ~CScriptGameResourceObjectScriptedThingBase() { --g_handles; }
    bool IsValid() const { return Valid; }
    void Release() { CHECK(Valid); Valid = false; Log("release"); }
    CScriptThing GetScriptThing() const { CHECK(Valid); return CScriptThing(7); }
    void Speak(const CScriptThing& to, const char* key, ETextGroupSelectionMethod m, bool a, bool b, bool c) { CHECK(to.Id == 1); CHECK(Valid); CHECK(!a && b && !c); Log("say %s %d", key, (int)m); Busy = g_speakFrames; }
    void MoveToPosition(const C3DVector& p, float sp, EScriptEntityMoveType t, bool a, bool b) { CHECK(Valid); CHECK(p.Y == 2.0f && sp == 1.0f && t == 1 && !a && b); Log("walk home"); Busy = g_moveBusy; }
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
    bool StartScriptingEntity(const CScriptThing& t, CScriptGameResourceObjectScriptedThingBase& h, EScriptAIPriority p) { CHECK(t.Id == 7 && p == SCRIPT_AI_PRIORITY_HIGHEST); CHECK(!h.Valid); if (g_startFails > 0) { g_startFails--; return false; } h.Valid = true; Log("start"); return true; }
    void StartMovieSequence(const CCharString& name, CScriptGameResourceObjectMovieBase&) { CHECK(name.Text[0] == 0); CHECK(g_movies > 0); Log("movie"); }
    void EntitySetInFaction(const CScriptThing& t, const CCharString& f) { CHECK(t.Id == 7); Log("faction %s", f.Text); }
    void SetIsPushableByHero(CScriptThing t, bool b) { CHECK(t.Id == 7 && b); Log("pushable"); }
    CScriptThing GetThingWithScriptName(const CCharString& n) { if (strcmp(n.Text, "VictimsFatherGuard") == 0) return CScriptThing(20); CHECK(strcmp(n.Text, "DemonDoorDoor") == 0); return CScriptThing(30); }
    void OpenDoor(const CScriptThing& d) { CHECK(d.Id == 30); Log("open door"); }
    void SetThingPersistent(const CScriptThing& d, bool b) { CHECK(d.Id == 30 && b); Log("persistent"); }
};
class CWideScreenMagicPauseEntities { public:
    explicit CWideScreenMagicPauseEntities(CGameScriptInterface&) { ++g_pauses; }
    ~CWideScreenMagicPauseEntities() { --g_pauses; }
};
namespace NScript {
struct CIsThingAlive { CScriptThing Thing; CIsThingAlive(const CScriptThing& t) : Thing(t) { } };
class CQ_RansomVictimScript { public:
    bool RescuedVictim, RansommerDied, RansomHelperStarted, RansomHelperSpoken, RansomHelpersAttacked, ToldGuardToWaitOutside;
    int DiedAt, SpokenAt;
    CQ_RansomVictimScript() : RescuedVictim(false), RansommerDied(false), RansomHelperStarted(false), RansomHelperSpoken(false),
        RansomHelpersAttacked(false), ToldGuardToWaitOutside(false), DiedAt(1 << 30), SpokenAt(1 << 30) { }
    void PlayCutscene(CCharString name, CCharString after) { CHECK(strcmp(after.Text, "NULL") == 0); CHECK(g_movies == 1 && g_pauses == 1); Log("cutscene %s", name.Text); }
    class CRansomHelper { public:
        CGameScriptInterface* Game;
        CScriptThing Me;
        CQ_RansomVictimScript& ParentClass;
        int PredicateId;
        CRansomHelper(CQ_RansomVictimScript& p) : Me(7), ParentClass(p), PredicateId(0) { }
        void SetPredicate(const CIsThingAlive& p) { PredicateId = p.Thing.Id; }
        bool IsActiveThreadTerminating() {
            if (g_frames >= ParentClass.DiedAt) ParentClass.RansommerDied = true;
            if (g_frames >= ParentClass.SpokenAt) ParentClass.RansomHelperSpoken = true;
            return Term(); }
        void Main();
    };
};
}
void NScript::CQ_RansomVictimScript::CRansomHelper::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game->EntitySetInFaction(Me,"FACTION_HERO");
    while (!ParentClass.RansomHelperStarted && !IsDistanceBetweenThingsUnder(Me,Game->GetHero(),10.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO")) || ParentClass.RansomHelpersAttacked) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ParentClass.RansomHelpersAttacked=true;
            Game->SetIsPushableByHero(Me,true);
            Game->EntitySetInFaction(Me,"FACTION_BANDITS");
            if (seh_me.IsValid()) seh_me.Release();
            while (true) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (seh_me.IsValid()) seh_me.Release();
    if (!ParentClass.RansomHelperStarted) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        ParentClass.RansomHelperStarted=true;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Game->GetThingWithScriptName("VictimsFatherGuard").IsAlive()) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            ParentClass.PlayCutscene("CS_RANSOM_CAVE_EXTERIOR_1","NULL");
            ParentClass.ToldGuardToWaitOutside=true;
        } else {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            ParentClass.PlayCutscene("CS_RANSOM_CAVE_EXTERIOR_2","NULL");
        }
        Game->OpenDoor(Game->GetThingWithScriptName("DemonDoorDoor"));
        Game->SetThingPersistent(Game->GetThingWithScriptName("DemonDoorDoor"),true);
        ParentClass.RansomHelperSpoken=true;
    }
    while (!ParentClass.RansomHelperSpoken) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CTimer ReturnToCaveTimer;
    bool BeenHit=false;
    while (!ParentClass.RansommerDied) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO")) || ParentClass.RansomHelpersAttacked) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ParentClass.RansomHelpersAttacked=true;
            break;
        }
        if (Me.MsgIsHitBy("")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid()) seh_me.Release();
            ReturnToCaveTimer=8;
            BeenHit=true;
        }
        if (BeenHit && ReturnToCaveTimer==0) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (!seh_me.IsPerformingScriptTask()) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                seh_me.MoveToPosition(Me.GetHomePos(),1.0f,(EScriptEntityMoveType)1,false,true);
            }
            if (IsDistanceFromThingToPositionUnder(Me,Me.GetHomePos(),2.0f)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                BeenHit=false;
            }
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO") && !BeenHit) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (ParentClass.RescuedVictim) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_B06_"+Me.GetDataString()+"_RESCUED_VICTIM_10",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_B06_"+Me.GetDataString()+"_YET_TO_PAY_10",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game->SetIsPushableByHero(Me,true);
    Game->EntitySetInFaction(Me,"FACTION_BANDITS");
    if (seh_me.IsValid()) seh_me.Release();
    while (true) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
}

static void Reset()
{
    g_log[0] = 0; g_frames = 0; g_iatt = 0; g_termAt = 1 << 30; g_termFrame = 1 << 30; g_heroHitAt = -1; g_heroHitKind = 0;
    g_anyHitAt = -1; g_talkAt = -1; g_talkAt2 = -1; g_guardAlive = true; g_nearAt = 1 << 30; g_homeAt = 1 << 30; g_timer = 0;
    g_health = 100.0f; g_startFails = 0; g_speakFrames = 0; g_moveBusy = 0;
}
static void Run(NScript::CQ_RansomVictimScript& parent)
{
    CGameScriptInterface game;
    {
        NScript::CQ_RansomVictimScript::CRansomHelper h(parent);
        h.Game = &game;
        h.Main();
        CHECK(h.PredicateId == 7);
        CHECK(g_live == 1);
        CHECK(g_handles == 0 && g_movies == 0 && g_pauses == 0 && g_timers == 0);
    }
    CHECK(g_live == 0);
}
static bool Has(const char* s) { return strstr(g_log, s) != 0; }
#define PRE "start;faction FACTION_HERO;"
#define CS1 "release;movie;cutscene CS_RANSOM_CAVE_EXTERIOR_1;open door;persistent;start;"

int main()
{
    // 1. terminating at the first check: one frame, nothing else
    { Reset(); NScript::CQ_RansomVictimScript p; g_termAt = 1; Run(p); CHECK(g_frames == 1 && g_log[0] == 0); }
    // 2. hero arrives (after one failed take-over), guard alive: EXTERIOR_1, wait-outside flag, door; the ransommer dies: bandit tail
    { Reset(); NScript::CQ_RansomVictimScript p; g_startFails = 1; g_nearAt = 3; p.DiedAt = 8; g_termFrame = 12; Run(p);
      CHECK(strcmp(g_log, PRE "release;movie;cutscene CS_RANSOM_CAVE_EXTERIOR_1;open door;persistent;start;pushable;faction FACTION_BANDITS;release;") == 0);
      CHECK(p.RansomHelperStarted && p.RansomHelperSpoken && p.ToldGuardToWaitOutside && !p.RansomHelpersAttacked); CHECK(g_frames == 12); }
    // 3. guard dead: EXTERIOR_2, no wait-outside flag
    { Reset(); NScript::CQ_RansomVictimScript p; g_guardAlive = false; g_nearAt = 2; g_termFrame = 6; Run(p);
      CHECK(Has("cutscene CS_RANSOM_CAVE_EXTERIOR_2;open door;persistent;start;")); CHECK(!p.ToldGuardToWaitOutside && p.RansomHelperSpoken); }
    // 4. another helper started the scene: no cutscene, wait for RansomHelperSpoken, then take over
    { Reset(); NScript::CQ_RansomVictimScript p; p.RansomHelperStarted = true; p.SpokenAt = 5; g_termFrame = 9; Run(p);
      CHECK(strcmp(g_log, PRE "release;start;") == 0); CHECK(g_frames == 9); }
    { Reset(); NScript::CQ_RansomVictimScript p; p.RansomHelperStarted = true; p.SpokenAt = 50; g_termFrame = 9; Run(p);
      CHECK(strcmp(g_log, PRE "release;") == 0); }
    // 5. hit by the hero while waiting: attacked flag, pushable, bandits, release, idle until terminated
    { Reset(); NScript::CQ_RansomVictimScript p; g_heroHitAt = 2; g_heroHitKind = 1; g_termFrame = 7; Run(p);
      CHECK(strcmp(g_log, PRE "pushable;faction FACTION_BANDITS;release;") == 0); CHECK(p.RansomHelpersAttacked && !p.RansomHelperStarted); }
    // 6. heal-life spell: no reaction; another special ability: reaction; another helper attacked: reaction
    { Reset(); NScript::CQ_RansomVictimScript p; g_heroHitAt = 2; g_heroHitKind = 2; g_termFrame = 5; Run(p); CHECK(strcmp(g_log, PRE) == 0); }
    { Reset(); NScript::CQ_RansomVictimScript p; g_heroHitAt = 2; g_heroHitKind = 3; g_termFrame = 5; Run(p); CHECK(Has("faction FACTION_BANDITS")); }
    { Reset(); NScript::CQ_RansomVictimScript p; p.RansomHelpersAttacked = true; g_termFrame = 5; Run(p); CHECK(Has("pushable;faction FACTION_BANDITS;release;")); CHECK(g_frames == 5); }
    // 7. main loop, hit by anyone: release, 8-second timer; when it runs out take over, walk home, reach home; then talk works again
    { Reset(); NScript::CQ_RansomVictimScript p; g_nearAt = 1; g_anyHitAt = 4; g_moveBusy = 3; g_homeAt = 16; g_talkAt = 14; g_talkAt2 = 18; g_termFrame = 22; Run(p);
      CHECK(strcmp(g_log, PRE CS1 "release;timer 8;start;walk home;release;start;release;start;release;start;release;start;walk home;release;start;movie;say TEXT_QST_B06_HELPER2_YET_TO_PAY_10 2;") == 0); }
    // 8. talk, victim rescued: the rescued line (speech waits a frame), then a dead helper says nothing
    { Reset(); NScript::CQ_RansomVictimScript p; p.RescuedVictim = true; g_nearAt = 1; g_talkAt = 4; g_speakFrames = 2; g_termFrame = 10; Run(p);
      CHECK(strcmp(g_log, PRE CS1 "release;start;movie;say TEXT_QST_B06_HELPER2_RESCUED_VICTIM_10 2;") == 0); }
    { Reset(); NScript::CQ_RansomVictimScript p; g_nearAt = 1; g_talkAt = 4; g_health = 0.0f; g_termFrame = 8; Run(p);
      CHECK(strcmp(g_log, PRE CS1 "release;start;movie;") == 0); }
    // 9. hit by the hero in the main loop: attacked flag, break to the bandit tail
    { Reset(); NScript::CQ_RansomVictimScript p; g_nearAt = 1; g_heroHitAt = 5; g_heroHitKind = 1; g_termFrame = 9; Run(p);
      CHECK(strcmp(g_log, PRE CS1 "pushable;faction FACTION_BANDITS;release;") == 0); CHECK(p.RansomHelpersAttacked); }
    // 10. terminating while speaking and while walking home: every object released
    { Reset(); NScript::CQ_RansomVictimScript p; g_nearAt = 1; g_talkAt = 4; g_speakFrames = 50; g_termFrame = 8; Run(p); CHECK(Has("YET_TO_PAY_10")); }
    { Reset(); NScript::CQ_RansomVictimScript p; g_nearAt = 1; g_anyHitAt = 3; g_moveBusy = 50; g_talkAt = 13; g_termFrame = 14; Run(p); CHECK(Has("timer 8;start;walk home;release;start;") && !Has("say")); }
    if (g_fail) printf("00de9690_TEST FAIL %d\n", g_fail); else printf("00de9690_TEST PASS\n");
    return g_fail ? 1 : 0;
}
