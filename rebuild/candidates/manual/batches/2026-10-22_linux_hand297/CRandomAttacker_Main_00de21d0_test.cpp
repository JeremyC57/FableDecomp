#include <cstdio>
#include <cstring>

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
};
static CWorld W;
static void ResetWorld()
{
    W.frame = 0; W.terminateAt = -1; W.hitAt = 1000; W.hitKind = 0; W.victimAlive = true; W.dataString[0] = 0;
    W.seed = 1; W.target = -1; W.targetSets = W.targetClears = 0; W.lastLine[0] = 0; W.lines = 0; W.nLog = 0;
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

// Model of CRandomAttacker::Main.
static void Main()
{
    NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    float distance = (float)(TestRand() % 12 + 8);
    while (!(HeroDistance() < distance)) {
        NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (W.victimAlive) {
        if (TestRand() % 2 == 0 && std::strcmp(W.dataString, "HeroOnly") != 0) GiveTarget(3);
        else GiveTarget(1);
    } else
        GiveTarget(1);
    for (;;) {
        if (IsHitBy() || (IsHitByAnySpecial() && !IsHitBySpecial14())) break;
        NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    ClearTarget();
    GiveTarget(1);
}

static bool Fail() { std::printf("00de21d0_TEST FAIL\n"); return false; }

static bool Scenario(bool victimAlive, const char* data, unsigned int seed, int hitKind, int expectFirst)
{
    ResetWorld(); W.victimAlive = victimAlive; std::strcpy(W.dataString, data); W.seed = seed; W.hitKind = hitKind;
    for (int i = 5; i < 64; ++i) W.heroDistance[i] = 7.0f;   // hero walks in at frame 5 (under any 8..19 radius)
    W.hitAt = 9; W.terminateAt = 30;
    // replay the rolls to know the first target
    unsigned int s = seed; s = s * 1103515245u + 12345u; s = s * 1103515245u + 12345u;
    int roll = (int)((s >> 16) & 0x7fff);
    int first = (victimAlive && roll % 2 == 0 && std::strcmp(data, "HeroOnly") != 0) ? 3 : 1;
    if (expectFirst != 0 && first != expectFirst) return Fail();
    Main();
    if (hitKind == 2) {   // ability 14 hits never count: the thread runs until it is terminated with the first target
        if (W.frame != 30 || W.target != first || W.targetSets != 1 || W.targetClears != 0) return Fail();
        return true;
    }
    if (W.frame != 9 || W.target != 1 || W.targetSets != 2 || W.targetClears != 1) return Fail();
    return true;
}

int main()
{
    // find seeds giving each roll parity
    unsigned int evenSeed = 0, oddSeed = 0;
    for (unsigned int s = 1; s < 100 && (!evenSeed || !oddSeed); ++s) {
        unsigned int t = s * 1103515245u + 12345u; t = t * 1103515245u + 12345u;
        if (((t >> 16) & 0x7fff) % 2 == 0) { if (!evenSeed) evenSeed = s; } else if (!oddSeed) oddSeed = s;
    }
    if (!Scenario(true, "", evenSeed, 0, 3)) return 1;
    if (!Scenario(true, "HeroOnly", evenSeed, 1, 1)) return 1;
    if (!Scenario(true, "", oddSeed, 0, 1)) return 1;
    if (!Scenario(false, "", evenSeed, 1, 1)) return 1;
    if (!Scenario(true, "", evenSeed, 2, 3)) return 1;
    ResetWorld(); W.terminateAt = 1; Main();
    if (W.targetSets != 0 || W.frame != 1) { Fail(); return 1; }
    std::printf("00de21d0_TEST PASS\n");
    return 0;
}
