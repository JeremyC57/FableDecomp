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

// Model of CRandomPopulationCreature::Main: attach Me to the evil (type 2) or good (types 0/1) village once, when that
// village is alive; take Me under scripted control and walk to the exit until within 2 units (each walk waits for the
// task to finish); non-evil types then lower the region's population scale by 0.05 clamped to [0,1]; remove Me and idle.
struct CParent { float CurrentPopulationScale[4]; long CurrentPopulationRegion; };
static bool g_evilAlive, g_goodAlive; static int g_attachedTo; static bool g_persistent, g_removed;
static float g_exitDistance; static int g_walks;
static void Main(CParent* parent, bool& attached, long myType)
{
    NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    if (!attached) {
        attached = true;
        if (myType == 2) { if (g_evilAlive) g_attachedTo = 2; }
        else if (myType == 0 || myType == 1) { if (g_goodAlive) g_attachedTo = 1; }
    }
    g_persistent = false;
    while (!StartScripting()) {
        NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    while (g_exitDistance > 2.0f) {
        NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        ++g_walks; W.taskBusyUntil = W.frame + 2;
        while (W.frame < W.taskBusyUntil) {
            NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        g_exitDistance -= 4.0f;
    }
    if (myType != 2) {
        float& scale = parent->CurrentPopulationScale[parent->CurrentPopulationRegion];
        scale -= 0.05f;
        if (scale < 0.0f) scale = 0.0f;
        else if (scale > 1.0f) scale = 1.0f;
    }
    g_removed = true;
    do NewScriptFrame(); while (!IsActiveThreadTerminating());
}

static void Reset(float exitDistance)
{
    ResetWorld(); g_attachedTo = 0; g_persistent = true; g_removed = false; g_exitDistance = exitDistance; g_walks = 0;
    W.terminateAt = 50;
}

int main()
{
    CParent p; p.CurrentPopulationRegion = 1; p.CurrentPopulationScale[0] = 0.5f; p.CurrentPopulationScale[1] = 0.5f; p.CurrentPopulationScale[2] = 0.02f; p.CurrentPopulationScale[3] = 1.5f;
    bool attached = false;
    g_evilAlive = true; g_goodAlive = true;
    Reset(10.0f); Main(&p, attached, 0);
    if (!attached || g_attachedTo != 1 || g_persistent || g_walks != 2 || !g_removed || W.frame != 50
        || p.CurrentPopulationScale[1] < 0.4499f || p.CurrentPopulationScale[1] > 0.4501f || p.CurrentPopulationScale[0] != 0.5f)
        { std::printf("00ec1ef0_TEST FAIL\n"); return 1; }
    Reset(1.0f); Main(&p, attached, 2);   // already attached: no village change; evil type keeps the scale
    if (g_attachedTo != 0 || g_walks != 0 || p.CurrentPopulationScale[1] < 0.4499f || p.CurrentPopulationScale[1] > 0.4501f)
        { std::printf("00ec1ef0_TEST FAIL\n"); return 1; }
    attached = false; Reset(1.0f); Main(&p, attached, 2);
    if (g_attachedTo != 2) { std::printf("00ec1ef0_TEST FAIL\n"); return 1; }
    attached = false; g_goodAlive = false; p.CurrentPopulationRegion = 2; Reset(1.0f); Main(&p, attached, 1);
    if (g_attachedTo != 0 || p.CurrentPopulationScale[2] != 0.0f) { std::printf("00ec1ef0_TEST FAIL\n"); return 1; }
    attached = false; p.CurrentPopulationRegion = 3; Reset(1.0f); Main(&p, attached, 3);
    if (g_attachedTo != 0 || p.CurrentPopulationScale[3] != 1.0f) { std::printf("00ec1ef0_TEST FAIL\n"); return 1; }
    Reset(100.0f); W.terminateAt = 6; Main(&p, attached, 0);
    if (g_removed || W.frame != 6) { std::printf("00ec1ef0_TEST FAIL\n"); return 1; }
    std::printf("00ec1ef0_TEST PASS\n");
    return 0;
}