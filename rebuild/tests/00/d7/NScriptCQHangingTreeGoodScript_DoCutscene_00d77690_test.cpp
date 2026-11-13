// Behaviour test for NScript::CQ_HangingTreeGoodScript::DoCutscene(long) (0x00D77690).  The function body below is the row's source
// (src/00d77690.cpp), run against a recording model of the script interface, the scripted-thing handles, the movie, the cast map and
// the CWideScreenMagicPauseEntities guard.  The expected event lists were read off the RETAIL disassembly in execution order (not from
// the source): dispatch `sub eax,0 ; je 0x00D78074 ; dec ; je 0x00D77CC8 ; dec ; jne 0x00D77A80` (0 / 1 / 2 / anything else returns);
//   case 0 (0x00D78074): handle IsValid 0x00D78091 / Release, StartScriptingEntity(GetHero(), handle, 4) 0x00D780B9 until true (frame
//     0x00D780C5, T 0x00D780CA), T 0x00D780F6, cast["HERO"] 0x00D78136, StartMovieSequence("") 0x00D7818C, PauseAll(true)
//     0x00D781A7, FixMovieSequenceCamera(true) 0x00D781B4, RunCutsceneMacro("CS_HANGINGTREE_GOOD", cast, 0, 0, false, true)
//     0x00D781E0, FixMovieSequenceCamera(false) 0x00D781F7; QuestStartScreenShown (+0x52) clear: T 0x00D78204,
//     KickOffQuestStartScreen("Q_HangingTreeGood", true, false) 0x00D7823D, QuestStartScreenShown = true 0x00D7824F;
//     PauseAll(false) 0x00D78258;
//   case 1 (0x00D77CC8): GetThingWithScriptName("HTG_Executioner") 0x00D77CED, ("HTG_Prisoner") 0x00D77D24, three handle waits
//     (executioner 0x00D77D77, prisoner 0x00D77DFA, hero 0x00D77E74), cast HERO / PRIS / EXEC 0x00D77EF0 / 0x00D77F2C / 0x00D77F68,
//     StartMovieSequence 0x00D77FAE, PauseAll(true), FixCamera(true), RunCutsceneMacro("CS_HANGINGTREE_GOOD_EXECUTION") 0x00D77FFF,
//     FixCamera(false) 0x00D78018, PauseAll(false) 0x00D78023;
//   case 2 (0x00D776B7): StartMovieSequence 0x00D776F6, PauseAll(true) 0x00D77712, GetThingWithScriptName("HTG_Prisoner") 0x00D7773D,
//     copies of CurrentGuardVector[0] / [1] (+0xAC), GetHero 0x00D7779C, its GetPos 0x00D777A6, CameraMoveToPosAndLookAtThing(pos +
//     (0, 0, 2), prisoner, 1) 0x00D777E3, StartScriptingEntity(guard1) 0x00D7783E, if guard2 alive StartScriptingEntity(guard2)
//     0x00D77869, StartScriptingEntity(prisoner) 0x00D7787D, GetNearestWithDefName(guard1, "REGION_EXIT_POINT") 0x00D778AA,
//     prisoner handle FollowThing(guard1, 1, true) 0x00D778CC, if guard2 alive guard2 handle FollowThing(prisoner, 1, true)
//     0x00D778F5, exit pos (null thing -> NullCoord), loop 0x00D77930: GetScriptThing 0x00D77941, IsDistanceFromThingToPositionOver(
//     thing, pos, 2) 0x00D7794F; while over: frame, T 0x00D779BB, MoveToPosition(pos, 0, WALK, false, true) 0x00D779D5,
//     IsPerformingScriptTask 0x00D779DE / frame / T 0x00D779F1, T 0x00D77A0D; after: T 0x00D77C53, CameraDefault 0x00D77C61,
//     PauseAll(false) 0x00D77CAC.  Every terminate exit runs the destructors (PauseAll(false) once the guard exists) and nothing else.
#include <stdio.h>
#include <string.h>

static char g_log[300][96];
static int g_logCount;
static int g_tCalls, g_stopAt, g_afterStop;
static bool g_stopped;
static void Put(const char* text)
{
    if (g_stopped && strcmp(text, "Pause(0)") != 0) ++g_afterStop;
    if (g_logCount < 300) { strncpy(g_log[g_logCount], text, 95); g_log[g_logCount][95] = 0; ++g_logCount; }
}
static long g_live;

// ---------------------------------------------------------------------------------------------------------------- model
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    const C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
static bool g_alive[16];
struct CThingImp { long Id; C3DVector Pos; };
static CThingImp g_imps[16];
class CScriptThing {
public:
    CThingImp* Data;
    CScriptThing() : Data(0) { ++g_live; }
    explicit CScriptThing(long id) : Data(id ? &g_imps[id] : 0) { ++g_live; }
    CScriptThing(const CScriptThing& o) : Data(o.Data) { ++g_live; }
    ~CScriptThing() { --g_live; }
    long Id() const { return Data ? Data->Id : 0; }
    bool IsAlive() const { char b[32]; sprintf(b, "IsAlive(%ld)", Id()); Put(b); return Data && g_alive[Data->Id]; }
    const C3DVector& GetPos() const { static C3DVector nullCoord(0, 0, 0); char b[32]; sprintf(b, "GetPos(%ld)", Id()); Put(b); return Data ? Data->Pos : nullCoord; }
};
class CCharString {
public:
    char Text[64];
    CCharString(const char* t) { strcpy(Text, t); ++g_live; }
    CCharString(const CCharString& o) { strcpy(Text, o.Text); ++g_live; }
    ~CCharString() { --g_live; }
    bool operator<(const CCharString& o) const { return strcmp(Text, o.Text) < 0; }
};
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST = 0, SCRIPT_AI_PRIORITY_HIGHEST = 4 };
enum EScriptEntityMoveType { ENTITY_MOVE_WALK = 0, ENTITY_MOVE_RUN = 1, ENTITY_MOVE_SNEAK = 2 };
static int g_validAnswer;
static int g_busyCount;
class CScriptGameResourceObjectScriptedThingBase {
public:
    long Entity;                                                     // the thing id bound by StartScriptingEntity (0 = none)
    CScriptGameResourceObjectScriptedThingBase() : Entity(0) { ++g_live; }
    CScriptGameResourceObjectScriptedThingBase(const CScriptGameResourceObjectScriptedThingBase& o) : Entity(o.Entity) { ++g_live; }
    ~CScriptGameResourceObjectScriptedThingBase() { --g_live; }
    CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& o) { Entity = o.Entity; return *this; }
    bool IsValid() const { Put("IsValid"); return g_validAnswer != 0; }
    void Release() { Put("Release"); }
    void FollowThing(const CScriptThing& t, float d, bool b) { char s[64]; sprintf(s, "Follow(%ld,%ld,%g,%d)", Entity, t.Id(), d, (int)b); Put(s); }
    void MoveToPosition(const C3DVector& p, float d, EScriptEntityMoveType m, bool a, bool b)
    {
        char s[96]; sprintf(s, "MoveTo(%ld,%g,%g,%g,%g,%d,%d,%d)", Entity, p.X, p.Y, p.Z, d, (int)m, (int)a, (int)b); Put(s);
    }
    bool IsPerformingScriptTask() const { Put("Busy?"); if (g_busyCount > 0) { --g_busyCount; return true; } return false; }
    CScriptThing GetScriptThing() const { char s[32]; sprintf(s, "GetScriptThing(%ld)", Entity); Put(s); return CScriptThing(Entity); }
};
class CScriptGameResourceObjectMovieBase {
public:
    CScriptGameResourceObjectMovieBase() { ++g_live; }
    ~CScriptGameResourceObjectMovieBase() { --g_live; }
};
namespace std {
template <class K, class V> class map {                         // the STLport map RunCutsceneMacro takes (what DoCutscene uses)
public:
    K* Keys[8]; V Values[8]; int Count;
    map() : Count(0) { ++g_live; }
    ~map() { for (int i = 0; i < Count; ++i) delete Keys[i]; --g_live; }
    V& operator[](const K& k)
    {
        char s[64]; sprintf(s, "cast[%s]", k.Text); Put(s);
        for (int i = 0; i < Count; ++i) if (!(*Keys[i] < k) && !(k < *Keys[i])) return Values[i];
        Keys[Count] = new K(k); return Values[Count++];
    }
};
}
typedef std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> TCast;
static void RunCutsceneMacro(const CCharString& name, TCast& cast, void* a, void* b, bool c, bool d)
{
    char s[200]; int n = sprintf(s, "Run(%s,%d,%d,%d,%d", name.Text, a ? 1 : 0, b ? 1 : 0, (int)c, (int)d);
    for (int i = 0; i < cast.Count; ++i) n += sprintf(s + n, ",%s=%ld", cast.Keys[i]->Text, cast.Values[i].Entity);
    sprintf(s + n, ")"); Put(s);
}
static bool g_distAnswers[16]; static int g_distCall;
static bool IsDistanceFromThingToPositionOver(const CScriptThing& t, const C3DVector& p, float d)
{
    char s[96]; sprintf(s, "DistOver(%ld,%g,%g,%g,%g)", t.Id(), p.X, p.Y, p.Z, d); Put(s);
    return g_distAnswers[g_distCall++];
}
static int g_startFails[16];
static CScriptThing* g_heroThing;                                      // StartScriptingEntity answers false this many times per thing
class CGameScriptInterfaceBase {
public:
    void NewScriptFrame() const { Put("F"); }
    CScriptThing& GetHero() { Put("GetHero"); return *g_heroThing; }
    CScriptThing GetThingWithScriptName(const CCharString& n) const
    {
        char s[64]; sprintf(s, "GetThing(%s)", n.Text); Put(s);
        return CScriptThing(strcmp(n.Text, "HTG_Prisoner") == 0 ? 2 : strcmp(n.Text, "HTG_Executioner") == 0 ? 3 : 0);
    }
    CScriptThing GetNearestWithDefName(const CScriptThing& t, const CCharString& n) const
    {
        char s[64]; sprintf(s, "GetNearest(%ld,%s)", t.Id(), n.Text); Put(s); return CScriptThing(6);
    }
    bool StartScriptingEntity(const CScriptThing& t, CScriptGameResourceObjectScriptedThingBase& h, EScriptAIPriority p)
    {
        char s[48]; sprintf(s, "Start(%ld,%d)", t.Id(), (int)p); Put(s);
        if (g_startFails[t.Id()] > 0) { --g_startFails[t.Id()]; return false; }
        h.Entity = t.Id(); return true;
    }
    void StartMovieSequence(const CCharString& n, CScriptGameResourceObjectMovieBase&) { char s[48]; sprintf(s, "Movie(%s)", n.Text); Put(s); }
    void FixMovieSequenceCamera(bool b) const { char s[16]; sprintf(s, "FixCam(%d)", (int)b); Put(s); }
    void PauseAllNonScriptedEntities(bool b) { char s[16]; sprintf(s, "Pause(%d)", (int)b); Put(s); }
    void KickOffQuestStartScreen(const CCharString& n, bool a, bool b) const { char s[64]; sprintf(s, "QuestStart(%s,%d,%d)", n.Text, (int)a, (int)b); Put(s); }
    void CameraMoveToPosAndLookAtThing(const C3DVector& p, const CScriptThing& t, float d) const
    {
        char s[96]; sprintf(s, "Camera(%g,%g,%g,%ld,%g)", p.X, p.Y, p.Z, t.Id(), d); Put(s);
    }
    void CameraDefault() { Put("CameraDefault"); }
};
class CWideScreenMagicPauseEntities {
    CGameScriptInterfaceBase* Game;
public:
    CWideScreenMagicPauseEntities(CGameScriptInterfaceBase& game) : Game(&game) { Game->PauseAllNonScriptedEntities(true); }
    ~CWideScreenMagicPauseEntities() { Game->PauseAllNonScriptedEntities(false); }
};
static CGameScriptInterfaceBase g_game;

namespace NScript {
class CQ_HangingTreeGoodScript {
public:
    CGameScriptInterfaceBase* Game;
    bool QuestStartScreenShown;
    CScriptThing CurrentGuardVector[2];
    CQ_HangingTreeGoodScript() : Game(&g_game), QuestStartScreenShown(false) { }
    bool IsActiveThreadTerminating()
    {
        Put("T");
        if (g_stopped) return true;
        if (++g_tCalls == g_stopAt) { g_stopped = true; return true; }
        return false;
    }
    void DoCutscene(long cutscene);
};
}

// ---------------------------------------------------------------------------------------------------------------- the row's body
void NScript::CQ_HangingTreeGoodScript::DoCutscene(long cutscene)
{
    switch (cutscene) {
    case 0: {
        CScriptGameResourceObjectScriptedThingBase seh_hero;
        if (seh_hero.IsValid()) seh_hero.Release();
        while (!Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> cast;
        cast["HERO"] = seh_hero;
        CScriptGameResourceObjectMovieBase movie;
        Game->StartMovieSequence("", movie);
        CWideScreenMagicPauseEntities pause(*Game);
        Game->FixMovieSequenceCamera(true);
        RunCutsceneMacro("CS_HANGINGTREE_GOOD", cast, 0, 0, false, true);
        Game->FixMovieSequenceCamera(false);
        if (!QuestStartScreenShown) {
            if (IsActiveThreadTerminating()) return;
            Game->KickOffQuestStartScreen("Q_HangingTreeGood", true, false);
            QuestStartScreenShown = true;
        }
        break;
    }
    case 1: {
        CScriptThing executioner = Game->GetThingWithScriptName("HTG_Executioner");
        CScriptThing prisoner = Game->GetThingWithScriptName("HTG_Prisoner");
        CScriptGameResourceObjectScriptedThingBase seh_executioner;
        if (seh_executioner.IsValid()) seh_executioner.Release();
        while (!Game->StartScriptingEntity(executioner, seh_executioner, SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        CScriptGameResourceObjectScriptedThingBase seh_prisoner;
        if (seh_prisoner.IsValid()) seh_prisoner.Release();
        while (!Game->StartScriptingEntity(prisoner, seh_prisoner, SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        CScriptGameResourceObjectScriptedThingBase seh_hero;
        if (seh_hero.IsValid()) seh_hero.Release();
        while (!Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> cast;
        cast["HERO"] = seh_hero;
        cast["PRIS"] = seh_prisoner;
        cast["EXEC"] = seh_executioner;
        CScriptGameResourceObjectMovieBase movie;
        Game->StartMovieSequence("", movie);
        CWideScreenMagicPauseEntities pause(*Game);
        Game->FixMovieSequenceCamera(true);
        RunCutsceneMacro("CS_HANGINGTREE_GOOD_EXECUTION", cast, 0, 0, false, true);
        Game->FixMovieSequenceCamera(false);
        break;
    }
    case 2: {
        CScriptGameResourceObjectMovieBase movie;
        Game->StartMovieSequence("", movie);
        CWideScreenMagicPauseEntities pause(*Game);
        CScriptThing prisoner = Game->GetThingWithScriptName("HTG_Prisoner");
        CScriptThing guard1 = CurrentGuardVector[0];
        CScriptThing guard2 = CurrentGuardVector[1];
        Game->CameraMoveToPosAndLookAtThing(Game->GetHero().GetPos() + C3DVector(0.0f, 0.0f, 2.0f), prisoner, 1.0f);
        CScriptGameResourceObjectScriptedThingBase seh_guard1;
        CScriptGameResourceObjectScriptedThingBase seh_guard2;
        CScriptGameResourceObjectScriptedThingBase seh_prisoner;
        Game->StartScriptingEntity(guard1, seh_guard1, SCRIPT_AI_PRIORITY_HIGHEST);
        if (guard2.IsAlive()) Game->StartScriptingEntity(guard2, seh_guard2, SCRIPT_AI_PRIORITY_HIGHEST);
        Game->StartScriptingEntity(prisoner, seh_prisoner, SCRIPT_AI_PRIORITY_HIGHEST);
        CScriptThing exit_point = Game->GetNearestWithDefName(guard1, "REGION_EXIT_POINT");
        seh_prisoner.FollowThing(guard1, 1.0f, true);
        if (guard2.IsAlive()) seh_guard2.FollowThing(prisoner, 1.0f, true);
        C3DVector exit_pos = exit_point.GetPos();
        while (IsDistanceFromThingToPositionOver(seh_guard1.GetScriptThing(), exit_pos, 2.0f)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            seh_guard1.MoveToPosition(exit_pos, 0.0f, ENTITY_MOVE_WALK, false, true);
            while (seh_guard1.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        Game->CameraDefault();
        break;
    }
    }
}

// ---------------------------------------------------------------------------------------------------------------- checks
// Each list is read off the retail code (header); "|" separates nothing, entries are compared one by one.
static const char* const g_case0[] = {
    "IsValid", "Release", "GetHero", "Start(1,4)", "F", "T", "GetHero", "Start(1,4)", "T",
    "cast[HERO]", "Movie()", "Pause(1)", "FixCam(1)", "Run(CS_HANGINGTREE_GOOD,0,0,0,1,HERO=1)", "FixCam(0)",
    "T", "QuestStart(Q_HangingTreeGood,1,0)", "Pause(0)", 0 };
static const char* const g_case0b[] = {                         // second run: the start screen was shown already
    "IsValid", "Release", "GetHero", "Start(1,4)", "F", "T", "GetHero", "Start(1,4)", "T", "cast[HERO]", "Movie()", "Pause(1)", "FixCam(1)",
    "Run(CS_HANGINGTREE_GOOD,0,0,0,1,HERO=1)", "FixCam(0)", "Pause(0)", 0 };
static const char* const g_case1[] = {
    "GetThing(HTG_Executioner)", "GetThing(HTG_Prisoner)",
    "IsValid", "Release", "Start(3,4)", "F", "T", "Start(3,4)", "T",
    "IsValid", "Release", "Start(2,4)", "F", "T", "Start(2,4)", "T",
    "IsValid", "Release", "GetHero", "Start(1,4)", "F", "T", "GetHero", "Start(1,4)", "T",
    "cast[HERO]", "cast[PRIS]", "cast[EXEC]", "Movie()", "Pause(1)", "FixCam(1)",
    "Run(CS_HANGINGTREE_GOOD_EXECUTION,0,0,0,1,HERO=1,PRIS=2,EXEC=3)", "FixCam(0)", "Pause(0)", 0 };
static const char* const g_case2[] = {
    "Movie()", "Pause(1)", "GetThing(HTG_Prisoner)", "GetHero", "GetPos(1)", "Camera(10,20,32,2,1)",
    "Start(4,4)", "IsAlive(5)", "Start(5,4)", "Start(2,4)", "GetNearest(4,REGION_EXIT_POINT)",
    "Follow(2,4,1,1)", "IsAlive(5)", "Follow(5,2,1,1)", "GetPos(6)",
    "GetScriptThing(4)", "DistOver(4,7,8,9,2)", "F", "T", "MoveTo(4,7,8,9,0,0,0,1)", "Busy?", "F", "T", "Busy?", "T",
    "GetScriptThing(4)", "DistOver(4,7,8,9,2)", "F", "T", "MoveTo(4,7,8,9,0,0,0,1)", "Busy?", "T",
    "GetScriptThing(4)", "DistOver(4,7,8,9,2)", "T", "CameraDefault", "Pause(0)", 0 };
static const char* const g_case2dead[] = {                      // guard 2 dead: no StartScriptingEntity / FollowThing for him
    "Movie()", "Pause(1)", "GetThing(HTG_Prisoner)", "GetHero", "GetPos(1)", "Camera(10,20,32,2,1)",
    "Start(4,4)", "IsAlive(5)", "Start(2,4)", "GetNearest(4,REGION_EXIT_POINT)", "Follow(2,4,1,1)", "IsAlive(5)", "GetPos(6)",
    "GetScriptThing(4)", "DistOver(4,7,8,9,2)", "F", "T", "MoveTo(4,7,8,9,0,0,0,1)", "Busy?", "F", "T", "Busy?", "T",
    "GetScriptThing(4)", "DistOver(4,7,8,9,2)", "F", "T", "MoveTo(4,7,8,9,0,0,0,1)", "Busy?", "T",
    "GetScriptThing(4)", "DistOver(4,7,8,9,2)", "T", "CameraDefault", "Pause(0)", 0 };

static void Setup(int stopAt)
{
    g_logCount = 0; g_tCalls = 0; g_stopAt = stopAt; g_stopped = false; g_afterStop = 0;
    for (int i = 0; i < 16; ++i) { g_imps[i].Id = i; g_imps[i].Pos = C3DVector((float)i, 0, 0); g_alive[i] = true; g_startFails[i] = 0; }
    g_imps[1].Pos = C3DVector(10, 20, 30); g_imps[6].Pos = C3DVector(7, 8, 9);
    g_startFails[1] = 1; g_startFails[2] = 1; g_startFails[3] = 1;
    g_validAnswer = 1; g_busyCount = 1; g_distCall = 0;
    static const bool dist[3] = { true, true, false };
    memcpy(g_distAnswers, dist, sizeof(dist));
}
static int Compare(const char* const* want, const char* tag)
{
    int n = 0;
    for (; want[n]; ++n)
        if (n >= g_logCount || strcmp(g_log[n], want[n]) != 0) { printf("%s mismatch at %d: got %s want %s\n", tag, n, n < g_logCount ? g_log[n] : "(end)", want[n]); return 1; }
    if (n != g_logCount) { printf("%s length %d v %d (extra %s)\n", tag, g_logCount, n, g_log[n]); return 1; }
    return 0;
}
static int Run(NScript::CQ_HangingTreeGoodScript& s, long which, int stopAt, bool guard2Alive)
{
    Setup(stopAt);
    g_alive[5] = guard2Alive;
    if (which == 2) { g_startFails[1] = g_startFails[2] = g_startFails[3] = 0; }   // case 2 does not wait for the handles
    long before = g_live;
    s.DoCutscene(which);
    if (g_live != before) return 1;
    if (g_afterStop) return 2;
    int pauses = 0;
    for (int i = 0; i < g_logCount; ++i) pauses += strcmp(g_log[i], "Pause(1)") == 0 ? 1 : strcmp(g_log[i], "Pause(0)") == 0 ? -1 : 0;
    if (pauses != 0) return 3;
    return 0;
}

int main()
{
    int bad = 0;
    CScriptThing hero(1);
    g_heroThing = &hero;
    NScript::CQ_HangingTreeGoodScript s;
    s.CurrentGuardVector[0] = CScriptThing(4);
    s.CurrentGuardVector[1] = CScriptThing(5);
    if (Run(s, 0, 0, true)) { printf("case0 run\n"); ++bad; }
    bad += Compare(g_case0, "case0");
    if (!s.QuestStartScreenShown) ++bad;
    if (Run(s, 0, 0, true)) ++bad;
    bad += Compare(g_case0b, "case0b");
    if (Run(s, 1, 0, true)) ++bad;
    bad += Compare(g_case1, "case1");
    if (Run(s, 2, 0, true)) ++bad;
    bad += Compare(g_case2, "case2");
    if (Run(s, 2, 0, false)) ++bad;
    bad += Compare(g_case2dead, "case2dead");
    for (long other = -2; other <= 6; ++other) {                   // only 0, 1 and 2 do anything
        if (other >= 0 && other <= 2) continue;
        Run(s, other, 0, true);
        if (g_logCount != 0) { printf("value %ld did something\n", other); ++bad; }
    }
    // a terminate request at the n-th check of each case: nothing but the destructors after it
    for (long which = 0; which <= 2; ++which) {
        s.QuestStartScreenShown = false;
        Run(s, which, 0, true);
        int total = g_tCalls;
        for (int k = 1; k <= total; ++k) {
            s.QuestStartScreenShown = false;
            int r = Run(s, which, k, true);
            if (r) { printf("case %ld stop at %d: %d\n", which, k, r); ++bad; }
        }
    }
    if (bad) { printf("00d77690_TEST FAIL\n"); return 1; }
    printf("00d77690_TEST PASS\n");
    return 0;
}