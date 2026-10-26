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

// Model of CRockTrollTrigger::Main: wait for the hero within the def's trigger distance; the first trigger to fire
// (RockTrollSpawned is per trigger) plays the troll cut-scene once per quest (ShownEarthTroll) with every non-scripted
// entity paused around it; the trigger then removes itself.
struct CParent { bool ShownEarthTroll; };
static long g_triggerDistance = 15;
static int g_cutscenes, g_pauseDepth, g_maxPause; static bool g_removed; static bool g_heroScripted;
static void Pause(bool on) { g_pauseDepth += on ? 1 : -1; if (g_pauseDepth > g_maxPause) g_maxPause = g_pauseDepth; }
static void Main(CParent* parent, bool& rockTrollSpawned)
{
    NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    long distance = g_triggerDistance;
    while (!(HeroDistance() < (float)distance)) {
        NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (!rockTrollSpawned) {
        rockTrollSpawned = true;
        if (!parent->ShownEarthTroll) {
            g_heroScripted = true;
            Pause(true);
            if (g_pauseDepth == 1) ++g_cutscenes;
            parent->ShownEarthTroll = true;
            Pause(false);
            g_heroScripted = false;
        }
    }
    g_removed = true;
}

int main()
{
    CParent parent; parent.ShownEarthTroll = false;
    bool spawnedA = false, spawnedB = false;
    g_cutscenes = g_pauseDepth = g_maxPause = 0; g_removed = false;
    ResetWorld(); for (int i = 6; i < 64; ++i) W.heroDistance[i] = 14.0f;
    Main(&parent, spawnedA);
    if (W.frame != 6 || g_cutscenes != 1 || !parent.ShownEarthTroll || !spawnedA || !g_removed || g_pauseDepth != 0 || g_maxPause != 1 || g_heroScripted)
        { std::printf("00e0ad40_TEST FAIL\n"); return 1; }
    g_removed = false; ResetWorld(); W.heroDistance[0] = 1.0f; W.heroDistance[1] = 1.0f;
    Main(&parent, spawnedB);   // a second trigger: no second cut-scene
    if (g_cutscenes != 1 || !spawnedB || !g_removed) { std::printf("00e0ad40_TEST FAIL\n"); return 1; }
    g_removed = false; ResetWorld(); W.terminateAt = 3;
    Main(&parent, spawnedA);   // never reached: nothing happens
    if (g_removed || W.frame != 3) { std::printf("00e0ad40_TEST FAIL\n"); return 1; }
    std::printf("00e0ad40_TEST PASS\n");
    return 0;
}