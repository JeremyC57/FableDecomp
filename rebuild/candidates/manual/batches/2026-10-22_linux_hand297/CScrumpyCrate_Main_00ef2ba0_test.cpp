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

// Model of CScrumpyCrate::Main: each frame, while the hero carries the crate it is invisible, the gate is open and a
// message says so; when he puts it down the gate closes and the crate is opaque again. Anyone else carrying it dims it
// to 0.2 (restored to 0.6 when it is put down).
static int g_carrier[64];         // per frame: 0 nobody, 1 hero, 2 someone else
static float g_alpha; static bool g_gateOpen; static int g_messages; static char g_lastMessage[80];
static int Carrier() { return g_carrier[W.frame < 64 ? W.frame : 63]; }
static void Message(const char* text) { std::strcpy(g_lastMessage, text); ++g_messages; }
static void Main()
{
    NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    while (!IsActiveThreadTerminating()) {
        if (Carrier() == 1) {
            g_alpha = 0.0f;
            Message("Hero's carrying the scrumpy crate!");
            g_gateOpen = true;
            while (Carrier() == 1) {
                NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            Message("Hero's no longer carrying the scrumpy crate!");
            g_gateOpen = false;
            g_alpha = 1.0f;
        } else if (Carrier() != 0) {
            g_alpha = 0.2f;
            Message("Someone other than the hero is carrying the scrumpy crate!");
            while (Carrier() != 0) {
                NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            g_alpha = 0.6f;
            Message("No one's carrying the scrumpy crate now!");
        }
        NewScriptFrame();
    }
}

int main()
{
    for (int i = 0; i < 64; ++i) g_carrier[i] = 0;
    for (int i = 3; i < 8; ++i) g_carrier[i] = 1;
    for (int i = 12; i < 15; ++i) g_carrier[i] = 2;
    ResetWorld(); g_alpha = 1.0f; g_gateOpen = false; g_messages = 0;
    W.terminateAt = 5; Main();
    if (g_alpha != 0.0f || !g_gateOpen || g_messages != 1) { std::printf("00ef2ba0_TEST FAIL\n"); return 1; }
    ResetWorld(); g_alpha = 1.0f; g_gateOpen = false; g_messages = 0;
    W.terminateAt = 10; Main();
    if (g_alpha != 1.0f || g_gateOpen || g_messages != 2 || std::strcmp(g_lastMessage, "Hero's no longer carrying the scrumpy crate!") != 0) { std::printf("00ef2ba0_TEST FAIL\n"); return 1; }
    ResetWorld(); g_alpha = 1.0f; g_messages = 0;
    W.terminateAt = 13; Main();
    if (g_alpha != 0.2f || g_messages != 3) { std::printf("00ef2ba0_TEST FAIL\n"); return 1; }
    ResetWorld(); g_alpha = 1.0f; g_messages = 0;
    W.terminateAt = 30; Main();
    if (g_alpha != 0.6f || g_messages != 4 || std::strcmp(g_lastMessage, "No one's carrying the scrumpy crate now!") != 0 || W.frame != 30) { std::printf("00ef2ba0_TEST FAIL\n"); return 1; }
    std::printf("00ef2ba0_TEST PASS\n");
    return 0;
}
