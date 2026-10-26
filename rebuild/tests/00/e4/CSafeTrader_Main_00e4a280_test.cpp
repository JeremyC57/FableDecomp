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

// Model of CSafeTrader::Main: under scripted control (priority 4) walk to the escape marker until within the def's
// escape distance; warn the hero once when he comes within the warning distance (the HATE line if he already hit Me);
// a counting hit marks AttackedByHero, scares Me and starts the precalculated route; finally fade out and kill Me.
struct CParent { bool SafeTraderWarnedHero; };
static float g_escapeDistance[64];
static int g_routes, g_scared; static bool g_killed; static float g_fadeTime;
static void Main(CParent* parent, bool& warnedHero, bool& attackedByHero)
{
    NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    while (!StartScripting()) {
        NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    long warningDistance = 12;
    while (g_escapeDistance[W.frame < 64 ? W.frame : 63] > 3.0f) {
        NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (!(W.frame < W.taskBusyUntil)) { ++W.moves; W.taskBusyUntil = W.frame + 5; }
        if (!warnedHero && HeroDistance() < (float)warningDistance) {
            warnedHero = true;
            parent->SafeTraderWarnedHero = true;
            Say(attackedByHero ? "TEXT_QST_011_SAFE_TRADER_WARNING_10_HATE" : "TEXT_QST_011_SAFE_TRADER_WARNING_10");
        }
        if (IsHitBy() || (IsHitByAnySpecial() && !IsHitBySpecial14())) {
            attackedByHero = true;
            ++g_scared;
            ++g_routes;
        }
    }
    g_killed = true; g_fadeTime = 2.0f;
}

static void Reset()
{
    ResetWorld(); g_routes = g_scared = 0; g_killed = false; g_fadeTime = 0;
    for (int i = 0; i < 64; ++i) g_escapeDistance[i] = i < 20 ? 50.0f : 1.0f;
}

int main()
{
    CParent parent; parent.SafeTraderWarnedHero = false;
    bool warned = false, attacked = false;
    Reset(); W.heroDistance[8] = 5.0f; W.heroDistance[9] = 5.0f;
    Main(&parent, warned, attacked);
    if (W.frame != 20 || !g_killed || !warned || !parent.SafeTraderWarnedHero || attacked || W.lines != 1 || W.moves != 4
        || std::strcmp(W.lastLine, "TEXT_QST_011_SAFE_TRADER_WARNING_10") != 0)
        { std::printf("00e4a280_TEST FAIL\n"); return 1; }
    warned = false; attacked = false;
    Reset(); W.hitAt = 4; W.hitKind = 1; W.heroDistance[10] = 1.0f;
    Main(&parent, warned, attacked);
    if (!attacked || g_scared != 17 || g_routes != 17 || std::strcmp(W.lastLine, "TEXT_QST_011_SAFE_TRADER_WARNING_10_HATE") != 0 || !g_killed)
        { std::printf("00e4a280_TEST FAIL\n"); return 1; }
    warned = false; attacked = false;
    Reset(); W.hitAt = 4; W.hitKind = 2;   // ability 14 does not count
    Main(&parent, warned, attacked);
    if (attacked || g_scared != 0 || warned) { std::printf("00e4a280_TEST FAIL\n"); return 1; }
    Reset(); W.terminateAt = 7;
    Main(&parent, warned, attacked);
    if (g_killed || W.frame != 7) { std::printf("00e4a280_TEST FAIL\n"); return 1; }
    std::printf("00e4a280_TEST PASS\n");
    return 0;
}