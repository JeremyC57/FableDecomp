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

// Model of CSP_SecretPassageGuard::Main: take Me under scripted control (retrying each frame), mutter now and then
// (hero within 30, one roll in 250) until the hero is seen or heard, then shout the surprised line, target the hero and
// release the scripting handle.
static void Main()
{
    NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    while (!StartScripting()) {
        NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    while (!CanSeeHero() && !CanHearHero()) {
        NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (HeroDistance() < 30.0f && TestRand() % 250 == 0) Say("TEXT_QST_055_GUARD_MOANING_ABOUT_STUFF");
    }
    Say("TEXT_QST_055_GUARD_SURPRISED_BY_HERO");
    GiveTarget(1);
    if (W.scripting) Release();
}

int main()
{
    ResetWorld(); W.scriptStartAt = 3; W.heardAt = 12;
    for (int i = 0; i < 64; ++i) W.heroDistance[i] = 20.0f;
    Main();
    if (W.frame != 12 || W.target != 1 || W.releases != 1 || W.scripting || std::strcmp(W.lastLine, "TEXT_QST_055_GUARD_SURPRISED_BY_HERO") != 0)
        { std::printf("00ded810_TEST FAIL\n"); return 1; }
    // a long wait within 30 units produces some muttering; out of range produces none
    int muttered = 0;
    for (unsigned int seed = 1; seed < 40; ++seed) {
        ResetWorld(); W.seed = seed; W.seenAt = 63;
        for (int i = 0; i < 64; ++i) W.heroDistance[i] = 10.0f;
        Main(); muttered += W.lines - 1;
        ResetWorld(); W.seed = seed; W.seenAt = 63;
        Main();
        if (W.lines != 1) { std::printf("00ded810_TEST FAIL\n"); return 1; }
    }
    if (muttered == 0) { std::printf("00ded810_TEST FAIL\n"); return 1; }
    ResetWorld(); W.scriptStartAt = 100; W.terminateAt = 5; Main();
    if (W.frame != 5 || W.scripting || W.targetSets != 0) { std::printf("00ded810_TEST FAIL\n"); return 1; }
    std::printf("00ded810_TEST PASS\n");
    return 0;
}