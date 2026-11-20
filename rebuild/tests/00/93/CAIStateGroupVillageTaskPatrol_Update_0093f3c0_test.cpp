// Real-object behaviour test for 0x0093F3C0 CAIStateGroup_VillageTaskPatrol::Update (links the REAL row object: vland's fallback link of
// the source object with this test). Every callee of the row is a stand-in bound by `/alternatename` to its PDB symbol; thiscall members
// are __fastcall stand-ins (ecx = this, edx unused, the same stack cleanup). Fixtures use the retail layouts the row reads: state group
// PBrain +0x04 (brain creature +0x18), PTCVillageMember +0x14, PTask +0x18 (CIntelligentPointer: vptr, PData); creature PhysicsTC +0x60,
// physics vtable slot 72 (+0x120) = GetRHSet() (Forward at +0x0C); task vtable slot 7 (+0x1C) = IsCreatureAssignedToTask, TaskLocked +0x18;
// track node Dead +0x91 bit 0, NextNode +0xD0, IsEnd +0xED. Each scenario's event trace is compared with a reference model of the
// patrol: take the task, unsheathe unless carrying, then per node move (callback repeat while in progress), lock the task, step to the
// next node (the track start after an end node), look at it when it lies ahead, idle until the wait frame, while the task is assigned.
#include <stdio.h>
#include <string.h>
#include <math.h>
#define ALT(a, b) comment(linker, "/alternatename:" a "=" b)
#pragma ALT("@CallUpdate@8", "?Update@CAIStateGroup_VillageTaskPatrol@@UAEXXZ")
#pragma ALT("?GetPVillage@CTCVillageMember@@QBEPAVCTCVillage@@XZ", "@GetPVillage@8")
#pragma ALT("?GetBestPatrolTaskForThing@CTCVillage@@QAEPAVCVillageTaskPatrol@@ABVCThing@@@Z", "@GetBest@12")
#pragma ALT("?SetPatrolTaskAsAcceptedByThing@CTCVillage@@QAE_NAAVCVillageTaskPatrol@@ABVCThing@@@Z", "@Accept@16")
#pragma ALT("?IsThingCarryingWeapon@CTCCarrying@@SI_NABVCThing@@@Z", "@Carrying@4")
#pragma ALT("??0CCreatureAction_UnsheatheWeapons@@QAE@AAVCThingCreatureBase@@@Z", "@UnsheatheCtor@12")
#pragma ALT("??1CCombatActionBase@@UAE@XZ", "@ActionDtor@8")
#pragma ALT("?AIAction_PerformActionAndWait@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@ABVCCreatureActionBase@@_N@Z", "@Perform@16")
#pragma ALT("?GetNearestNode@CVillageTaskPatrol@@QBEPBVCThingTrackNode@@ABVC3DVector@@@Z", "@Nearest@12")
#pragma ALT("?PeekTrackStartNode@CThingTrackNode@@QBEPBV1@XZ", "@StartNode@8")
#pragma ALT("?GetPos@CThing@@QBEABVC3DVector@@XZ", "@GetPos@8")
#pragma ALT("?AIAction_MoveToPosProximity@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@VC3DVector@@MAA_NW4EMovementType@@_N44PBVCThing@@4M@Z", "@Move@60")
#pragma ALT("?AIAction_LookAtPos@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@VC3DVector@@JM@Z", "@LookAt@32")
#pragma ALT("?AIAction_MinionIdle@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@XZ", "@Idle@8")
#pragma ALT("?GetPWorldSeed@CAIStateGroupBase@@IBEPAKXZ", "@Seed@8")
#pragma ALT("?GFFloatRandom@@YIMMPAK@Z", "@FloatRandom@8")
#pragma ALT("?GetFrame@CWorld@@SIJXZ", "@GetFrame@0")
#pragma ALT("?Terminate@CAIStateGroupBase@@QAEXXZ", "@Terminate@8")
#pragma ALT("??0CBaseIntelligentPointer@@QAE@XZ", "@IPCtor@8")
#pragma ALT("??1CBaseIntelligentPointer@@UAE@XZ", "@IPDtor@8")
#pragma ALT("?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ", "@IPGet@8")
#pragma ALT("?SetPItem@CBaseIntelligentPointer@@QAEXPAVCBaseObject@@@Z", "@IPSet@12")
#pragma ALT("??_G?$CIntelligentPointer@$$CBVCThingTrackNode@@@@UAEPAXI@Z", "@IPDel@12")
#pragma ALT("??_G?$CIntelligentPointer@VCVillageTaskPatrol@@@@UAEPAXI@Z", "@IPDel@12")
#pragma ALT("?ConstantFPS@CMainGameComponent@@1JA", "_g_fps")
#pragma ALT("?GFastOneOverSqrtTable@@3PAKA", "_g_table")
extern "C" { long g_fps = 30; unsigned long g_table[128]; }

static int g_fail = 0, g_scn = 0;
static void Check(bool ok, int line) { if (!ok) { g_fail = 1; printf("check failed at line %d (scenario %d)\n", line, g_scn); } }
#define CHECK(c) Check((c) ? true : false, __LINE__)

struct Vec { float x, y, z; };
struct RHSet { Vec up, forward; };
struct Phys { void** vtbl; };
struct Thing { unsigned char p00[0x60]; Phys* phys; unsigned char p64[0x91 - 0x64]; unsigned char dead; unsigned char p92[0xD0 - 0x92];
               Thing* next; Thing* prev; unsigned char pd8[0xED - 0xD8]; bool isEnd; unsigned char pee[2]; Vec pos; };
typedef char ThingCheck[(size_t)&((Thing*)0)->isEnd == 0xED && (size_t)&((Thing*)0)->next == 0xD0 && (size_t)&((Thing*)0)->dead == 0x91 ? 1 : -1];
struct IP { void* vtbl; void* item; };
struct Task { void** vtbl; unsigned char p04[0x14]; bool locked; unsigned char p19[3]; };
typedef char TaskCheck[(size_t)&((Task*)0)->locked == 0x18 ? 1 : -1];
struct Brain { unsigned char p00[0x18]; Thing* creature; };
struct Group { void* vtbl; Brain* brain; unsigned char p08[0xC]; void* member; IP task; };
typedef char GroupCheck[(size_t)&((Group*)0)->task == 0x18 && (size_t)&((Group*)0)->member == 0x14 ? 1 : -1];

// scenario controls
static struct { bool noTask, refuse, carrying; int deadAtMove, moveFailAt, inProgress, lookTerminateAt, idleFailAt, assignedChecks; } P;
static char g_trace[4096]; static int g_tn;
static void T(char c) { if (g_tn < 4095) g_trace[g_tn++] = c; g_trace[g_tn] = 0; }
static Thing g_creature, g_node[3]; static Phys g_phys; static void* g_physVtbl[73]; static RHSet g_rh;
static Task g_task; static void* g_taskVtbl[9]; static Brain g_brain; static Group g_group; static int g_village, g_member;
static int g_moves, g_moveCalls, g_looks, g_idles, g_assigned, g_frame, g_actionLive; static unsigned long g_seed;
static const Thing* g_lastMoveNode;

extern "C" {
void __fastcall CallUpdate(Group* g, int);
void* __fastcall GetPVillage(void* m, int) { CHECK(m == &g_member); T('G'); return &g_village; }
Task* __fastcall GetBest(void* v, int, const Thing* t) { CHECK(v == &g_village && t == &g_creature); T('B'); return P.noTask ? 0 : &g_task; }
bool __fastcall Accept(void* v, int, Task* task, const Thing* t) { CHECK(v == &g_village && task == &g_task && t == &g_creature); T('A'); return !P.refuse; }
bool __fastcall Carrying(const Thing* t) { CHECK(t == &g_creature); T('C'); return P.carrying; }
void* __fastcall UnsheatheCtor(void* a, int, Thing* c) { CHECK(c == &g_creature); T('U'); ++g_actionLive; return a; }
void __fastcall ActionDtor(void* a, int) { T('u'); --g_actionLive; }
int __fastcall Perform(Group* g, int, void* action, int wait) { CHECK(g == &g_group && (wait & 0xFF) == 0 && g_actionLive == 1); T('P'); return 2; }
const Thing* __fastcall Nearest(Task* t, int, const Vec* pos) { CHECK(t == &g_task && pos == &g_creature.pos); T('N'); return &g_node[1]; }
const Thing* __fastcall StartNode(const Thing* n, int) { CHECK(n == &g_node[2]); T('S'); return &g_node[0]; }
const Vec* __fastcall GetPos(const Thing* t, int) { return &t->pos; }
int __fastcall Move(Group* g, int, int repeat, Vec pos, float prox, bool* reached, int movement, int b1, int b2, int b3, const void* avoid, int b4, float speed)
{
    CHECK(g == &g_group && repeat == 2 && prox == 0.5f && !*reached && movement == 3 && (b1 & 0xFF) == 1 && (b2 & 0xFF) == 0 && (b3 & 0xFF) == 1 &&
          avoid == 0 && (b4 & 0xFF) == 1 && speed == 0.0f);
    CHECK(g_lastMoveNode && pos.x == g_lastMoveNode->pos.x && pos.y == g_lastMoveNode->pos.y && pos.z == g_lastMoveNode->pos.z);
    T('M'); ++g_moveCalls;
    if (g_moveCalls % (P.inProgress + 1) != 0) return 4;
    ++g_moves;
    return g_moves == P.moveFailAt ? 1 : 2;
}
int __fastcall LookAt(Group* g, int, int repeat, Vec pos, long flags, float speed)
{
    CHECK(g == &g_group && repeat == 1 && flags == 9 && speed == 0.0027777f); T('L'); ++g_looks;
    return g_looks == P.lookTerminateAt ? 1 : 2;
}
int __fastcall Idle(Group* g, int) { T('I'); ++g_idles; return g_idles == P.idleFailAt ? 1 : 2; }
unsigned long* __fastcall Seed(Group* g, int) { CHECK(g == &g_group); return &g_seed; }
float __fastcall FloatRandom(float range, unsigned long* seed) { CHECK(range == 1.0f && seed == &g_seed); T('R'); return 0.25f; }
long __fastcall GetFrame() { return ++g_frame; }
void __fastcall Terminate(Group* g, int) { CHECK(g == &g_group); T('T'); }
void __fastcall IPCtor(IP* p, int) { p->item = 0; }
void __fastcall IPDtor(IP* p, int) { if (p != &g_group.task) T('d'); }
void* __fastcall IPGet(const IP* p, int) { return p->item; }
void __fastcall IPSet(IP* p, int, void* item) { p->item = item; if (p != &g_group.task) g_lastMoveNode = (const Thing*)item; }
void* __fastcall IPDel(IP* p, int, unsigned) { return p; }
const RHSet* __fastcall GetRH(Phys* p, int) { CHECK(p == &g_phys); return &g_rh; }
bool __fastcall IsAssigned(Task* t, int, const Thing* c)
{
    CHECK(t == &g_task && c == &g_creature); T('a'); ++g_assigned;
    if (g_assigned == P.deadAtMove) ((Thing*)g_lastMoveNode)->dead = 1;
    return g_assigned < P.assignedChecks;
}
}

static void InitTable()
{
    for (int i = 0; i < 128; ++i) {
        int e = (i >> 6) ? 127 : 128; double f = ((i & 63) + 0.5) / 64.0;
        double x = ldexp(1.0 + f, e - 127), y = 1.0 / sqrt(x);
        int ey = (0x17c - e) >> 1; double m = ldexp(y, 127 - ey) - 1.0;
        int k = (int)(m * 256.0 + 0.5); if (k < 0) k = 0; if (k > 255) k = 255; g_table[i] = k;
    }
}

// Reference model: the expected trace of one run.
static bool g_expectLocked;
static void Model(char* out)
{
    g_expectLocked = false;
    int n = 0, moves = 0, moveCalls = 0, looks = 0, idles = 0, assigned = 0, frame = 0; int node = 1; bool dead[3] = { 0, 0, 0 };
#define E(c) (out[n++] = (c), out[n] = 0)
#define ASSIGNED() (E('a'), ++assigned, (assigned == P.deadAtMove ? (dead[node] = 1) : 0), assigned < P.assignedChecks)
    E('G'); E('B'); E('d');
    if (P.noTask) { E('T'); return; }
    E('A'); if (P.refuse) { E('T'); return; }
    E('C'); if (!P.carrying) { E('U'); E('P'); E('u'); }
    E('N');
    while (ASSIGNED()) {
        int r;
        for (;;) {
            if (!ASSIGNED() || dead[node]) { E('T'); E('d'); return; }
            E('M'); ++moveCalls;
            if (moveCalls % (P.inProgress + 1) != 0) continue;
            ++moves; r = moves == P.moveFailAt ? 1 : 2; break;
        }
        if (r != 2 || !ASSIGNED()) { E('T'); E('d'); return; }
        g_expectLocked = true;
        if (node == 2) { E('S'); node = 0; } else node = node + 1;
        E('d');
        const Vec& cp = g_creature.pos; const Vec& np = g_node[node].pos;
        if ((np.x - cp.x) * g_rh.forward.x + (np.y - cp.y) * g_rh.forward.y > 0.0f) {
            E('L'); ++looks; if (looks == P.lookTerminateAt) { E('T'); E('d'); return; }
        }
        E('R');
        long wait = (long)(++frame + (0.25 + 0.5) * g_fps);
        while (++frame < wait) { E('I'); ++idles; if (idles == P.idleFailAt || !ASSIGNED()) { E('T'); E('d'); return; } }
    }
    E('d'); E('T');
#undef ASSIGNED
#undef E
}

static void Run()
{
    ++g_scn; g_tn = 0; g_trace[0] = 0;
    g_moves = g_moveCalls = g_looks = g_idles = g_assigned = g_frame = g_actionLive = 0; g_lastMoveNode = 0;
    for (int i = 0; i < 3; ++i) g_node[i].dead = 0;
    memset(&g_task, 0, sizeof g_task); g_task.vtbl = g_taskVtbl; g_taskVtbl[7] = (void*)IsAssigned;
    g_group.task.item = (void*)0x1234; g_group.brain = &g_brain; g_brain.creature = &g_creature; g_group.member = &g_member;
    char expect[4096]; Model(expect);
    CallUpdate(&g_group, 0);
    CHECK(strcmp(g_trace, expect) == 0);
    if (strcmp(g_trace, expect)) printf("  got    %s\n  expect %s\n", g_trace, expect);
    CHECK(g_group.task.item == (P.noTask ? 0 : (void*)&g_task));
    CHECK(g_task.locked == g_expectLocked);
    CHECK(g_actionLive == 0);
}

int main()
{
    InitTable();
    g_creature.phys = &g_phys; g_phys.vtbl = g_physVtbl; g_physVtbl[72] = (void*)GetRH;
    g_creature.pos.x = 0; g_creature.pos.y = 0; g_creature.pos.z = 20;
    g_rh.forward.x = 1; g_rh.forward.y = 0; g_rh.forward.z = 0.5f;
    g_node[0].pos.x = -4; g_node[0].pos.y = 1;  g_node[0].pos.z = 2;
    g_node[1].pos.x = 3;  g_node[1].pos.y = -2; g_node[1].pos.z = 9;
    g_node[2].pos.x = 6;  g_node[2].pos.y = 7;  g_node[2].pos.z = -1;
    g_node[0].next = &g_node[1]; g_node[1].next = &g_node[2]; g_node[2].next = 0; g_node[2].isEnd = true;
    memset(&P, 0, sizeof P); P.noTask = true; Run();
    memset(&P, 0, sizeof P); P.refuse = true; Run();
    memset(&P, 0, sizeof P); P.assignedChecks = 1; Run();                                  // never assigned: unsheathe then leave
    memset(&P, 0, sizeof P); P.carrying = true; P.assignedChecks = 400; P.inProgress = 2; P.idleFailAt = 60; Run();
    memset(&P, 0, sizeof P); P.assignedChecks = 400; P.moveFailAt = 3; Run();
    memset(&P, 0, sizeof P); P.carrying = true; P.assignedChecks = 400; P.lookTerminateAt = 2; Run();
    memset(&P, 0, sizeof P); P.assignedChecks = 400; P.deadAtMove = 30; Run();
    memset(&P, 0, sizeof P); P.assignedChecks = 50; P.inProgress = 1; Run();               // runs until the task is unassigned
    memset(&P, 0, sizeof P); P.assignedChecks = 3; Run();                                  // unassigned after the move
    for (int k = 4; k < 40; k += 3) { memset(&P, 0, sizeof P); P.assignedChecks = k; P.carrying = (k & 1) != 0; P.inProgress = k % 3; Run(); }
    if (g_fail) { printf("0093f3c0_TEST FAIL\n"); return 1; }
    printf("0093f3c0_TEST PASS (%d scenarios)\n", g_scn);
    return 0;
}