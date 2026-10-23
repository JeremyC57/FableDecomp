#include <cstdio>
#include <cstring>
#include <cstdlib>

// --- test harness: a scripted world advanced one frame per NewScriptFrame ---
struct CThing {
    int id;                 // 0 = none, 1 = hero, 2 = Me, 3+ = named things
    CThing() : id(0) {}
    explicit CThing(int i) : id(i) {}
};
struct CWorld {
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

// Model of CSP_ScreamerTrigger::Main: wait for the hero within 10 units, spawn <data string> undead soldiers near
// random spawn points (each named EnemySeekingHero and set on the hero), then wait until every EnemySeekingHero is dead.
struct CCreature { int spawnPoint; bool targetsHero; int diesAt; };
static CCreature g_creatures[16]; static int g_nCreatures;
static int g_spawnPoints = 3;
static bool AllDead() { for (int i = 0; i < g_nCreatures; ++i) if (W.frame < g_creatures[i].diesAt) return false; return true; }
static void Main()
{
    while (!(HeroDistance() < 10.0f)) {
        NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    long count = std::atoi(W.dataString);
    for (long i = 0; i < count; ++i) {
        if (IsActiveThreadTerminating()) return;
        CCreature& c = g_creatures[g_nCreatures++];
        c.spawnPoint = TestRand() % g_spawnPoints;
        c.targetsHero = true;
        c.diesAt = 10 + 3 * (int)i;
    }
    while (!AllDead()) {
        NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
}

int main()
{
    ResetWorld(); g_nCreatures = 0; std::strcpy(W.dataString, "4");
    for (int i = 4; i < 64; ++i) W.heroDistance[i] = 9.5f;
    Main();
    if (g_nCreatures != 4 || W.frame != 19) { std::printf("00def3b0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 4; ++i)
        if (!g_creatures[i].targetsHero || g_creatures[i].spawnPoint < 0 || g_creatures[i].spawnPoint >= 3) { std::printf("00def3b0_TEST FAIL\n"); return 1; }
    ResetWorld(); g_nCreatures = 0; std::strcpy(W.dataString, "0");
    for (int i = 2; i < 64; ++i) W.heroDistance[i] = 1.0f;
    Main();
    if (g_nCreatures != 0 || W.frame != 2) { std::printf("00def3b0_TEST FAIL\n"); return 1; }
    ResetWorld(); g_nCreatures = 0; std::strcpy(W.dataString, "2"); W.terminateAt = 6;
    Main();
    if (g_nCreatures != 0 || W.frame != 6) { std::printf("00def3b0_TEST FAIL\n"); return 1; }
    std::printf("00def3b0_TEST PASS\n");
    return 0;
}
