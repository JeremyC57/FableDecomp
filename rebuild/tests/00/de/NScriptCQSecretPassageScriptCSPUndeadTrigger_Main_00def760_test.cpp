#include <cstdio>
#include <cstring>
#include <cstdlib>

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

// Model of CSP_UndeadTrigger::Main: start the parent's door switcher once, wait for the hero within 2.5 units, turn
// every SP_UndeadSpawnPoint<data> marker into a hero-hunting undead soldier (replacing the marker in the vector; a
// BlockADoor marker also blocks the doors), wait until they are all dead, unblock the doors and remove Me.
struct CParent { bool DoorsBlocked; bool DoorsBeingDone; int switchers; };
struct CSlot { char name[32]; char data[16]; bool isCreature; int diesAt; };
static CSlot g_world[8]; static int g_nWorld;
static CSlot g_vec[8]; static int g_nVec;
static bool g_removed;
static bool AllDead() { for (int i = 0; i < g_nVec; ++i) if (!g_vec[i].isCreature || W.frame < g_vec[i].diesAt) return false; return true; }
static void Main(CParent* parent)
{
    if (!parent->DoorsBeingDone) {
        if (IsActiveThreadTerminating()) return;
        ++parent->switchers;
        parent->DoorsBeingDone = true;
    }
    while (!(HeroDistance() < 2.5f)) {
        NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    char wanted[48];
    std::strcpy(wanted, "SP_UndeadSpawnPoint"); std::strcat(wanted, W.dataString);
    g_nVec = 0;
    for (int i = 0; i < g_nWorld; ++i) if (std::strcmp(g_world[i].name, wanted) == 0) g_vec[g_nVec++] = g_world[i];
    for (int i = 0; i < g_nVec; ++i) {
        if (IsActiveThreadTerminating()) return;
        CSlot creature; std::strcpy(creature.name, "EnemySeekingHero"); creature.data[0] = 0; creature.isCreature = true; creature.diesAt = 8 + i;
        if (std::strcmp(g_vec[i].data, "BlockADoor") == 0) parent->DoorsBlocked = true;
        g_vec[i] = creature;
    }
    while (!AllDead()) {
        NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    parent->DoorsBlocked = false;
    g_removed = true;
}

static void AddMarker(const char* name, const char* data)
{
    CSlot& s = g_world[g_nWorld++]; std::strcpy(s.name, name); std::strcpy(s.data, data); s.isCreature = false; s.diesAt = 0;
}

int main()
{
    ResetWorld(); g_nWorld = 0; g_removed = false; std::strcpy(W.dataString, "A");
    AddMarker("SP_UndeadSpawnPointA", ""); AddMarker("SP_UndeadSpawnPointB", "BlockADoor"); AddMarker("SP_UndeadSpawnPointA", "BlockADoor");
    for (int i = 3; i < 64; ++i) W.heroDistance[i] = 2.0f;
    CParent parent; parent.DoorsBlocked = false; parent.DoorsBeingDone = false; parent.switchers = 0;
    W.terminateAt = 9;   // first pass: killed while the undead are alive -> doors stay blocked
    Main(&parent);
    if (parent.switchers != 1 || !parent.DoorsBeingDone || !parent.DoorsBlocked || g_nVec != 2 || !g_vec[0].isCreature || !g_vec[1].isCreature || g_removed)
        { std::printf("00def760_TEST FAIL\n"); return 1; }
    ResetWorld(); std::strcpy(W.dataString, "A");
    for (int i = 3; i < 64; ++i) W.heroDistance[i] = 2.0f;
    Main(&parent);       // second run: the switcher is not spawned again; everything dies at frame 9
    if (parent.switchers != 1 || parent.DoorsBlocked || !g_removed || W.frame != 9) { std::printf("00def760_TEST FAIL\n"); return 1; }
    std::printf("00def760_TEST PASS\n");
    return 0;
}