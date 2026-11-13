// Behaviour test for NScript::CQ_HobbeCaveScript::CHobbeTorrent::Main (0x00D92380).  The function body below is the row's source
// (src/00d92380.cpp), run against a recording model of the script interface, the script timer manager and the vector.  The
// expected event lists were read off the RETAIL disassembly (0x00D92380..0x00D92919) in execution order, not from the source:
//   wait  : NymphAngry +0x6c / ReleaseBrat +0x4f tested at 0x00D92387 / 0x00D92393, NewScriptFrame 0x00D923A2, T 0x00D923A7,
//           T after the wait 0x00D923C0; timer slot 87 (take a timer) 0x00D923D7; `NumberToGenerate > 0` 0x00D923DF / 0x00D926EF;
//   body  : NewScriptFrame 0x00D923FD, T 0x00D92402, GetAllThingsWithScriptName("HobbeRoaming") 0x00D9242E, size >
//           HC_HobbeMaxNumberInRegion 0x00D9245D (unsigned, jbe);
//   cap   : NewScriptFrame 0x00D92475, T 0x00D9247A, found = false 0x00D92487; pass: NewScriptFrame 0x00D92495, T 0x00D9249A,
//           per element T 0x00D924C9, IsDead (slot 76) 0x00D924DF, when dead T 0x00D924EB and found = true; T 0x00D92522,
//           `if (!found)` again 0x00D9252F; T 0x00D92539, erase(begin, end) 0x00D92554, GetAll 0x00D92578, size test 0x00D925A7;
//   spawn : T 0x00D925B5, GetTimer 0x00D925CB (== 0), GetHero 0x00D925E6, IsDistanceBetweenThingsOver(Me, hero, 8.0) 0x00D925F0,
//           Me.GetPos 0x00D92606, IsCameraPosOnScreen 0x00D9260D (must be false), T 0x00D9261D, "CREATURE_HOBBE_GRUNT" 0x00D92635,
//           rand 0x00D9263A, rand % 12 through the index table 0x00D92930 {0,1,2,2,2,3,3,4,4,4,4,4} and the jump table 0x00D9291C
//           (SPELLCASTER, LIEUTENANT, GRUNT_SPIKE, GRUNT_HAMMER, GRUNT), CreateCreatureNearby(def, Me.GetPos(), 5.0,
//           "HobbeRoaming", false) 0x00D926BA, --NumberToGenerate 0x00D926D2, SetTimer(2) 0x00D926E0;
//   end   : T 0x00D926FC, RemoveThing(Me, false, true) 0x00D928CF, vector freed, timer slot 88 (drop the timer) 0x00D9290C.
// Every terminate exit after the timer is taken destroys the vector and drops the timer; exits before it touch nothing.
#include <stdio.h>
#include <string.h>
#include <new>

// ---------------------------------------------------------------------------------------------------------------- recorder
static char g_log[400][64];
static int g_logCount;
static int g_tCalls, g_stopAt, g_afterStop;
static bool g_stopped;
static void Put(const char* text)
{
    if (g_stopped && strcmp(text, "Dereg") != 0) ++g_afterStop;
    if (g_logCount < 400) { strncpy(g_log[g_logCount], text, 63); g_log[g_logCount][63] = 0; ++g_logCount; }
}

// ---------------------------------------------------------------------------------------------------------------- model
class C3DVector { public: float X, Y, Z; };
static long g_live;                                          // live CScriptThing objects
static int g_deadAfter[16];                                  // IsDead answers false this many times, then true
class CScriptThing {
public:
    long Id;
    C3DVector Pos;
    CScriptThing() : Id(0) { Pos.X = Pos.Y = Pos.Z = 0.0f; ++g_live; }
    CScriptThing(long id) : Id(id) { Pos.X = (float)id; Pos.Y = Pos.Z = 0.0f; ++g_live; }
    CScriptThing(const CScriptThing& o) : Id(o.Id), Pos(o.Pos) { ++g_live; }
    CScriptThing& operator=(const CScriptThing& o) { Id = o.Id; Pos = o.Pos; return *this; }
    ~CScriptThing() { --g_live; }
    const C3DVector& GetPos() const { char b[32]; sprintf(b, "GetPos(%ld)", Id); Put(b); return Pos; }
    bool IsDead() const
    {
        char b[32]; sprintf(b, "IsDead(%ld)", Id); Put(b);
        if (g_deadAfter[Id] > 0) { --g_deadAfter[Id]; return false; }
        return true;
    }
};
namespace std {
template <class T> class vector {                           // the STLport vector the row uses (only what Main touches)
public:
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector() { for (T* p = _M_start; p != _M_finish; ++p) p->~T(); delete[] (char*)_M_start; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T& operator[](unsigned long i) { return _M_start[i]; }
    void clear() { Put("Clear"); for (T* p = _M_start; p != _M_finish; ++p) p->~T(); _M_finish = _M_start; }
    void push_back(const T& x)
    {
        if (_M_finish == _M_end_of_storage) {
            unsigned long n = size(), cap = n ? n * 2 : 4;
            T* p = (T*)new char[cap * sizeof(T)];
            for (unsigned long i = 0; i < n; ++i) { new (&p[i]) T(_M_start[i]); _M_start[i].~T(); }
            delete[] (char*)_M_start; _M_start = p; _M_finish = p + n; _M_end_of_storage = p + cap;
        }
        new (_M_finish) T(x); ++_M_finish;
    }
};
}
class CCharString {
public:
    char Text[64];
    CCharString(const char* t) { strcpy(Text, t); }
    const CCharString& operator=(const char* t) { strcpy(Text, t); return *this; }
};
class CScriptDef { public: long HC_HobbeMaxNumberInRegion; };
static CScriptDef g_def;
static CScriptDef* pGlobalScriptDef = &g_def;

static int g_getAllCall, g_getAllCounts[16], g_getAllIds[16][8];
static int g_timerAnswers[16], g_timerCall, g_distAnswers[16], g_distCall, g_screenAnswers[16], g_screenCall;
static int g_randValue;
static int g_bratAfterFrames, g_frames;
struct CQ_HobbeCaveScript { bool ReleaseBrat; bool NymphAngry; };
static CQ_HobbeCaveScript g_parent;
static CScriptThing* g_hero;
static int g_timersLive;

class CGameScriptInterfaceBase {
public:
    void NewScriptFrame() const { Put("F"); if (++g_frames >= g_bratAfterFrames) g_parent.ReleaseBrat = true; }
    long GetAllThingsWithScriptName(const CCharString& name, std::vector<CScriptThing>& out) const
    {
        char b[64]; sprintf(b, "GetAll(%s)", name.Text); Put(b);
        int k = g_getAllCall++;
        for (int i = 0; i < g_getAllCounts[k]; ++i) out.push_back(CScriptThing(g_getAllIds[k][i]));
        return g_getAllCounts[k];
    }
    CScriptThing& GetHero() { Put("GetHero"); return *g_hero; }
    bool IsCameraPosOnScreen(const C3DVector& p) const
    {
        char b[48]; sprintf(b, "OnScreen(%g)", p.X); Put(b);
        return g_screenAnswers[g_screenCall++] != 0;
    }
    CScriptThing CreateCreatureNearby(const CCharString& def, const C3DVector& p, float r, const CCharString& name, bool b) const
    {
        char t[200]; sprintf(t, "Create(%s,%g,%g,%s,%d)", def.Text, p.X, r, name.Text, (int)b); Put(t);
        return CScriptThing(99);
    }
    void RemoveThing(const CScriptThing& t, bool a, bool b) const { char s[48]; sprintf(s, "Remove(%ld,%d,%d)", t.Id, (int)a, (int)b); Put(s); }
    long TakeTimer() { Put("Reg"); ++g_timersLive; return 7; }
    void DropTimer(long i) { Put("Dereg"); if (i == 7) --g_timersLive; }
    void SetTimer(long i, long t) { char s[32]; sprintf(s, "SetTimer(%ld,%ld)", i, t); Put(s); }
    long GetTimer(long i) const { char s[32]; sprintf(s, "GetTimer(%ld)", i); Put(s); return g_timerAnswers[g_timerCall++]; }
};
static CGameScriptInterfaceBase g_game;
static CGameScriptInterfaceBase* pDSTGame = &g_game;
class CTimer {
public:
    long TimerIndex;
    CTimer() : TimerIndex(pDSTGame->TakeTimer()) { }
    ~CTimer() { pDSTGame->DropTimer(TimerIndex); }
    CTimer& operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
    long GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
};
static bool IsDistanceBetweenThingsOver(const CScriptThing& a, const CScriptThing& b, float d)
{
    char s[64]; sprintf(s, "DistOver(%ld,%ld,%g)", a.Id, b.Id, d); Put(s);
    return g_distAnswers[g_distCall++] != 0;
}

namespace NScript {
class CEntityScriptBase {
public:
    CGameScriptInterfaceBase* Game;
    CScriptThing Me;
    CEntityScriptBase() : Game(&g_game), Me(5) { }
    bool IsActiveThreadTerminating() const
    {
        Put("T");
        if (g_stopped) return true;
        if (++g_tCalls == g_stopAt) { g_stopped = true; return true; }
        return false;
    }
};
class CHobbeTorrent : public CEntityScriptBase {
public:
    CQ_HobbeCaveScript* ParentClass;
    long NumberToGenerate;
    static int rand() { Put("rand"); return g_randValue; }
    void Main();
};
}

// ---------------------------------------------------------------------------------------------------------------- the row's body
void NScript::CHobbeTorrent::Main()
{
    while (!ParentClass->NymphAngry && !ParentClass->ReleaseBrat) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    CTimer spawn_timer;
    std::vector<CScriptThing> hobbes;
    while (NumberToGenerate > 0) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        Game->GetAllThingsWithScriptName("HobbeRoaming", hobbes);
        while (hobbes.size() > pGlobalScriptDef->HC_HobbeMaxNumberInRegion) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            bool one_dead = false;
            while (!one_dead) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
                for (unsigned long i = 0; i < hobbes.size(); ++i) {
                    if (IsActiveThreadTerminating()) return;
                    if (hobbes[i].IsDead()) {
                        if (IsActiveThreadTerminating()) return;
                        one_dead = true;
                    }
                }
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            hobbes.clear();
            Game->GetAllThingsWithScriptName("HobbeRoaming", hobbes);
        }
        if (IsActiveThreadTerminating()) return;
        if (spawn_timer.GetTime() == 0 && IsDistanceBetweenThingsOver(Me, Game->GetHero(), 8.0f) && !Game->IsCameraPosOnScreen(Me.GetPos())) {
            if (IsActiveThreadTerminating()) return;
            CCharString creature("CREATURE_HOBBE_GRUNT");
            switch (rand() % 12) {
            case 0:
                creature = "CREATURE_HOBBE_SPELLCASTER";
                break;
            case 1:
                creature = "CREATURE_HOBBE_LIEUTENANT";
                break;
            case 2:
            case 3:
            case 4:
                creature = "CREATURE_HOBBE_GRUNT_SPIKE";
                break;
            case 5:
            case 6:
                creature = "CREATURE_HOBBE_GRUNT_HAMMER";
                break;
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
                creature = "CREATURE_HOBBE_GRUNT";
                break;
            }
            Game->CreateCreatureNearby(creature, Me.GetPos(), 5.0f, "HobbeRoaming", false);
            --NumberToGenerate;
            spawn_timer = 2;
        }
    }
    if (IsActiveThreadTerminating()) return;
    Game->RemoveThing(Me, false, true);
}

// ---------------------------------------------------------------------------------------------------------------- checks

// Scenario A, read off the retail instructions (see the header): one wait frame, a 4-hobbe cap pass that needs two passes over
// the list before hobbe 2 is dead, a spawn (rand 6 -> GRUNT_HAMMER), then a busy timer, a near hero, an on-screen spot, and a
// second spawn (rand 0 -> SPELLCASTER) that uses up NumberToGenerate.
static const char* const g_expectA[] = {
    "F", "T", "T", "Reg",
    // iteration 1
    "F", "T", "GetAll(HobbeRoaming)",
    "F", "T",
    "F", "T", "T", "IsDead(1)", "T", "IsDead(2)", "T", "IsDead(3)", "T", "IsDead(4)", "T",
    "F", "T", "T", "IsDead(1)", "T", "IsDead(2)", "T", "T", "IsDead(3)", "T", "IsDead(4)", "T",
    "T", "Clear", "GetAll(HobbeRoaming)",
    "T", "GetTimer(7)", "GetHero", "DistOver(5,6,8)", "GetPos(5)", "OnScreen(5)", "T", "rand", "GetPos(5)",
    "Create(CREATURE_HOBBE_GRUNT_HAMMER,5,5,HobbeRoaming,0)", "SetTimer(7,2)",
    // iteration 2: timer still running
    "F", "T", "GetAll(HobbeRoaming)", "T", "GetTimer(7)",
    // iteration 3: hero within 8
    "F", "T", "GetAll(HobbeRoaming)", "T", "GetTimer(7)", "GetHero", "DistOver(5,6,8)",
    // iteration 4: on screen
    "F", "T", "GetAll(HobbeRoaming)", "T", "GetTimer(7)", "GetHero", "DistOver(5,6,8)", "GetPos(5)", "OnScreen(5)",
    // iteration 5: spawn
    "F", "T", "GetAll(HobbeRoaming)", "T", "GetTimer(7)", "GetHero", "DistOver(5,6,8)", "GetPos(5)", "OnScreen(5)", "T", "rand",
    "GetPos(5)", "Create(CREATURE_HOBBE_SPELLCASTER,5,5,HobbeRoaming,0)", "SetTimer(7,2)",
    // end
    "T", "Remove(5,0,1)", "Dereg",
    0
};

static long g_number;
static void Setup(int stopAt, int randValue)
{
    g_logCount = 0; g_tCalls = 0; g_stopAt = stopAt; g_stopped = false; g_afterStop = 0;
    g_getAllCall = 0; g_timerCall = 0; g_distCall = 0; g_screenCall = 0; g_frames = 0; g_bratAfterFrames = 1;
    g_parent.NymphAngry = false; g_parent.ReleaseBrat = false;
    g_def.HC_HobbeMaxNumberInRegion = 3;
    memset(g_getAllCounts, 0, sizeof(g_getAllCounts));
    g_getAllCounts[0] = 4; g_getAllIds[0][0] = 1; g_getAllIds[0][1] = 2; g_getAllIds[0][2] = 3; g_getAllIds[0][3] = 4;
    g_getAllCounts[1] = 2; g_getAllIds[1][0] = 1; g_getAllIds[1][1] = 3;
    memset(g_deadAfter, 0, sizeof(g_deadAfter));
    g_deadAfter[1] = 100; g_deadAfter[2] = 1; g_deadAfter[3] = 100; g_deadAfter[4] = 100;
    static const int timer[] = { 0, 1, 0, 0, 0 }, dist[] = { 1, 0, 1, 1 }, screen[] = { 0, 1, 0 };
    memcpy(g_timerAnswers, timer, sizeof(timer)); memcpy(g_distAnswers, dist, sizeof(dist)); memcpy(g_screenAnswers, screen, sizeof(screen));
    g_randValue = randValue;
}

static int RunA(int stopAt, int randValue, long number)
{
    Setup(stopAt, randValue);
    long liveBefore = g_live;
    {
        NScript::CHobbeTorrent t;
        t.ParentClass = &g_parent;
        t.NumberToGenerate = number;
        t.Main();
        g_number = t.NumberToGenerate;
    }
    if (g_live != liveBefore) return 1;
    if (g_timersLive != 0) return 2;
    if (g_afterStop != 0) return 3;
    return 0;
}

int main()
{
    CScriptThing hero(6);
    g_hero = &hero;
    int bad = 0;
    // (1) the full scenario, once with rand 6 (first spawn GRUNT_HAMMER checked) and once with rand 0 (second spawn SPELLCASTER)
    {
        // first spawn checked with rand 6
        RunA(0, 6, 2);
        int n = 0;
        for (; g_expectA[n]; ++n) {
            const char* want = g_expectA[n];
            if (strstr(want, "SPELLCASTER")) want = "Create(CREATURE_HOBBE_GRUNT_HAMMER,5,5,HobbeRoaming,0)";
            if (n >= g_logCount || strcmp(g_log[n], want) != 0) { printf("A mismatch at %d: got %s want %s\n", n, n < g_logCount ? g_log[n] : "(end)", want); ++bad; break; }
        }
        if (n != g_logCount) { printf("A length %d v %d\n", g_logCount, n); ++bad; }
        if (g_number != 0) ++bad;
        // second spawn checked with rand 0
        RunA(0, 0, 2);
        for (n = 0; g_expectA[n]; ++n) {
            const char* want = g_expectA[n];
            if (strstr(want, "GRUNT_HAMMER")) want = "Create(CREATURE_HOBBE_SPELLCASTER,5,5,HobbeRoaming,0)";
            if (n >= g_logCount || strcmp(g_log[n], want) != 0) { printf("A0 mismatch at %d\n", n); ++bad; break; }
        }
    }
    // (2) rand % 12 -> creature, per retail's index table {0,1,2,2,2,3,3,4,4,4,4,4} and jump table order
    {
        static const char* const defs[5] = { "CREATURE_HOBBE_SPELLCASTER", "CREATURE_HOBBE_LIEUTENANT", "CREATURE_HOBBE_GRUNT_SPIKE",
                                             "CREATURE_HOBBE_GRUNT_HAMMER", "CREATURE_HOBBE_GRUNT" };
        static const int index[12] = { 0, 1, 2, 2, 2, 3, 3, 4, 4, 4, 4, 4 };
        for (int r = 0; r < 30; ++r) {
            RunA(0, r, 1);
            char want[128]; sprintf(want, "Create(%s,5,5,HobbeRoaming,0)", defs[index[r % 12]]);
            bool found = false;
            for (int i = 0; i < g_logCount; ++i) if (strcmp(g_log[i], want) == 0) found = true;
            if (!found) { printf("rand %d wrong creature\n", r); ++bad; }
        }
    }
    // (3) a terminate request at the n-th check stops the script there: nothing but the cleanup after it, the vector's things
    //     destroyed and the timer released (only when it was taken)
    {
        RunA(0, 6, 2);
        int total = g_tCalls;
        for (int k = 1; k <= total; ++k) {
            int r = RunA(k, 6, 2);
            if (r != 0) { printf("stop at %d: %d\n", k, r); ++bad; }
            int last = g_logCount - 1;
            bool timerOn = false;
            for (int i = 0; i < g_logCount; ++i) if (strcmp(g_log[i], "Reg") == 0) timerOn = true;
            if (timerOn ? strcmp(g_log[last], "Dereg") != 0 || strcmp(g_log[last - 1], "T") != 0 : strcmp(g_log[last], "T") != 0) {
                printf("stop at %d: bad tail\n", k); ++bad;
            }
        }
        int expectedT = 0;
        for (int i = 0; g_expectA[i]; ++i) if (strcmp(g_expectA[i], "T") == 0) ++expectedT;
        if (total != expectedT) { printf("T count %d v %d\n", total, expectedT); ++bad; }
    }
    // (4) NumberToGenerate already 0: no frame, straight to RemoveThing; NymphAngry alone ends the wait
    {
        Setup(0, 6);
        g_parent.NymphAngry = true;
        NScript::CHobbeTorrent t;
        t.ParentClass = &g_parent;
        t.NumberToGenerate = 0;
        t.Main();
        static const char* const want[] = { "T", "Reg", "T", "Remove(5,0,1)", 0 };
        for (int i = 0; want[i]; ++i) if (i >= g_logCount || strcmp(g_log[i], want[i]) != 0) { printf("zero mismatch %d\n", i); ++bad; break; }
    }
    if (bad) { printf("00d92380_TEST FAIL\n"); return 1; }
    printf("00d92380_TEST PASS\n");
    return 0;
}