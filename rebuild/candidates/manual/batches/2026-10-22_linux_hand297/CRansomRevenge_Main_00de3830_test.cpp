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

// Model of CRansomRevenge::Main: a 10..29 radius, the victim (alive, even roll) or the hero as target, one time in
// three a line naming who it is going for, then wait for a counting hit and retarget the hero.
static void Say(const char* line) { std::strcpy(W.lastLine, line); ++W.lines; }
static void Main()
{
    NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    float distance = (float)(TestRand() % 20 + 10);
    while (!(HeroDistance() < distance)) {
        NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (W.victimAlive && TestRand() % 2 == 0) {
        GiveTarget(3);
        if (TestRand() % 3 == 0) Say("TEXT_QST_B06_ASSASSIN_ON_GOING_FOR_VICTIM");
    } else {
        GiveTarget(1);
        if (TestRand() % 3 == 0) Say("TEXT_QST_B06_ASSASSIN_ON_GOING_FOR_HERO");
    }
    for (;;) {
        if (IsHitBy() || (IsHitByAnySpecial() && !IsHitBySpecial14())) break;
        NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    ClearTarget();
    GiveTarget(1);
}

static unsigned int Next(unsigned int s) { return s * 1103515245u + 12345u; }
static int Roll(unsigned int s) { return (int)((s >> 16) & 0x7fff); }

int main()
{
    int covered = 0;
    for (unsigned int seed = 1; seed < 200; ++seed) {
        for (int alive = 0; alive < 2; ++alive) {
            ResetWorld(); W.seed = seed; W.victimAlive = alive != 0;
            for (int i = 3; i < 64; ++i) W.heroDistance[i] = 9.0f;
            W.hitAt = 6; W.hitKind = 1; W.terminateAt = 40;
            unsigned int s1 = Next(seed), s2 = Next(s1), s3 = Next(s2);
            bool victim = alive && Roll(s2) % 2 == 0;
            bool talk = victim ? Roll(s3) % 3 == 0 : (alive ? Roll(s3) % 3 == 0 : Roll(s2) % 3 == 0);
            Main();
            if (W.frame != 6 || W.target != 1 || W.targetSets != 2 || W.targetClears != 1) { std::printf("00de3830_TEST FAIL\n"); return 1; }
            if (talk != (W.lines == 1)) { std::printf("00de3830_TEST FAIL\n"); return 1; }
            if (talk) {
                const char* want = victim ? "TEXT_QST_B06_ASSASSIN_ON_GOING_FOR_VICTIM" : "TEXT_QST_B06_ASSASSIN_ON_GOING_FOR_HERO";
                if (std::strcmp(W.lastLine, want) != 0) { std::printf("00de3830_TEST FAIL\n"); return 1; }
                covered |= victim ? 1 : 2;
            }
        }
    }
    if (covered != 3) { std::printf("00de3830_TEST FAIL\n"); return 1; }
    ResetWorld(); W.terminateAt = 1; Main();
    if (W.targetSets != 0) { std::printf("00de3830_TEST FAIL\n"); return 1; }
    std::printf("00de3830_TEST PASS\n");
    return 0;
}
