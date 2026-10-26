#include <cstdio>
#include <cstring>

// --- test harness: a scripted world advanced one frame per NewScriptFrame ---
class CThing { public:
    int id;                 // 0 = none, 1 = hero, 2 = Me, 3+ = named things
    CThing() : id(0) {}
    explicit CThing(int i) : id(i) {}
};
class CWorld { public:
    int frame;
    int terminateAt;        // IsActiveThreadTerminating() turns true from this frame on (-1 = never)
    float heroDistance[64]; // hero distance per frame
    int hitAt;              // frame from which Me is hit
    int hitKind;            // 0 = plain hit, 1 = special ability (not 14), 2 = ability 14 only
    bool victimAlive;
    char dataString[32];
    unsigned int seed;
    int target;             // best enemy target id (0 = cleared)
    int targetSets, targetClears;
    char lastLine[96]; int lines;
    int log[32]; int nLog;
    int seenAt, heardAt, scriptStartAt; bool scripting; int releases; int taskBusyUntil; int moves;
};
static CWorld W;
static void ResetWorld()
{
    W.frame = 0; W.terminateAt = -1; W.hitAt = 1000; W.hitKind = 0; W.victimAlive = true; W.dataString[0] = 0;
    W.seed = 1; W.target = -1; W.targetSets = W.targetClears = 0; W.lastLine[0] = 0; W.lines = 0; W.nLog = 0;
    W.seenAt = W.heardAt = 1000; W.scriptStartAt = 0; W.scripting = false; W.releases = 0; W.taskBusyUntil = 0; W.moves = 0;
    for (int i = 0; i < 64; ++i) W.heroDistance[i] = 100.0f;
}
static int TestRand() { W.seed = W.seed * 1103515245u + 12345u; return (int)((W.seed >> 16) & 0x7fff); }
static void NewScriptFrame() { ++W.frame; }
static bool IsActiveThreadTerminating() { return W.terminateAt >= 0 && W.frame >= W.terminateAt; }
static float HeroDistance() { return W.heroDistance[W.frame < 64 ? W.frame : 63]; }
static bool IsHitBy() { return W.frame >= W.hitAt && W.hitKind == 0; }
static bool IsHitByAnySpecial() { return W.frame >= W.hitAt && W.hitKind != 0; }
static bool IsHitBySpecial14() { return W.frame >= W.hitAt && W.hitKind == 2; }
static void GiveTarget(int id) { W.target = id; ++W.targetSets; }
static void ClearTarget() { W.target = 0; ++W.targetClears; }
static bool CanSeeHero() { return W.frame >= W.seenAt; }
static bool CanHearHero() { return W.frame >= W.heardAt; }
static bool StartScripting() { if (W.frame >= W.scriptStartAt) { W.scripting = true; return true; } return false; }
static void Release() { if (W.scripting) { W.scripting = false; ++W.releases; } }
static void Say(const char* line) { std::strcpy(W.lastLine, line); ++W.lines; }

// Model of CSUMMONED_CREATURE::Main: count Me into the arena totals, then each frame score the hero's hits: a
// flourish (+5, needs the crowd timer at 0) or a decapitation (+7, needs the timer at 8 or less) plays the crowd-loop
// sound for the current base level and restarts the timer at 10; any other counting hit is +2.
struct CParent { long TotalCreatures[4]; long ExtraCreatures; long NewCrowdPoints; long NewCrowdBaseLevel; int timerSetFrame; long timerValue; };
static int g_hit[64];   // per frame: 0 none, 1 flourish, 2 decapitate, 3 plain, 4 special, 5 ability 14
static int g_sounds; static long g_lastSoundLevel;
static long GetTime(const CParent* p) { long t = p->timerValue - (W.frame - p->timerSetFrame); return t > 0 ? t : 0; }
static void SetTime(CParent* p, long v) { p->timerValue = v; p->timerSetFrame = W.frame; }
static void Main(CParent* p)
{
    NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    ++p->TotalCreatures[0];
    ++p->ExtraCreatures;
    while (!IsActiveThreadTerminating()) {
        int hit = g_hit[W.frame < 64 ? W.frame : 63];
        if (hit == 1) {
            if (GetTime(p) <= 0) { ++g_sounds; g_lastSoundLevel = p->NewCrowdBaseLevel; SetTime(p, 10); p->NewCrowdPoints += 5; }
        } else if (hit == 2) {
            if (GetTime(p) <= 8) { ++g_sounds; g_lastSoundLevel = p->NewCrowdBaseLevel; SetTime(p, 10); p->NewCrowdPoints += 7; }
        } else if (hit == 3 || hit == 4) {
            p->NewCrowdPoints += 2;
        }
        NewScriptFrame();
    }
}

int main()
{
    CParent p; p.TotalCreatures[0] = 3; p.ExtraCreatures = 0; p.NewCrowdPoints = 0; p.NewCrowdBaseLevel = 2; p.timerSetFrame = 0; p.timerValue = 0;
    for (int i = 0; i < 64; ++i) g_hit[i] = 0;
    g_hit[2] = 1;    // flourish, timer 0: +5, timer 10
    g_hit[3] = 2;    // decapitate, timer 9: nothing
    g_hit[4] = 2;    // decapitate, timer 8: +7, timer 10
    g_hit[5] = 1;    // flourish, timer 9: nothing
    g_hit[6] = 3;    // plain hit: +2
    g_hit[7] = 4;    // special ability: +2
    g_hit[8] = 5;    // ability 14: nothing
    g_hit[20] = 1;   // flourish, timer expired: +5
    g_sounds = 0; g_lastSoundLevel = -1;
    ResetWorld(); W.terminateAt = 30;
    Main(&p);
    if (p.TotalCreatures[0] != 4 || p.ExtraCreatures != 1 || p.NewCrowdPoints != 5 + 7 + 2 + 2 + 5 || g_sounds != 3 || g_lastSoundLevel != 2 || W.frame != 30)
        { std::printf("00f1b220_TEST FAIL\n"); return 1; }
    ResetWorld(); W.terminateAt = 1; Main(&p);
    if (p.TotalCreatures[0] != 4) { std::printf("00f1b220_TEST FAIL\n"); return 1; }
    std::printf("00f1b220_TEST PASS\n");
    return 0;
}