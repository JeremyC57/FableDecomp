// Behaviour test for CTCCombat::UpdateGetHitCounterAttack (0x00733CF0): links the row object (its same-TU GetCombatAbilityData,
// IsActive / IsValid / SetAsActive and the const component-map LowerBound run for real) with stand-ins for the out-of-line callees.
// Fixtures are raw memory in the retail layout:
//   CTCCombat: ParentThing +0x04, CombatAbilities (STLport multimap) +0x70: header node pointer (left = leftmost, right = rightmost,
//     parent = root); node: parent +4, left +8, right +0x0C, key +0x10, CCountedPointer<CCombatAbilityData> data +0x14, CCombatAbility
//     RepeatTimer +0x1C / ActiveTimer +0x20.
//   CCombatAbilityData: PercentageChance +4, SecondsDuration +8 (float), SecondsToWaitBeforeRepeat +0x0C, InterfaceType +0x14.
//   CThing / CThingCreatureBase: UsingComponents +0x20 (TCI_ENTITY_EVENTS 0x42 = bit 2 of +0x28), Components +0x44 (sorted
//   {type, CTCBase*} pairs: start +0x44, finish +0x48), PCurrentAction +0x100 (vtable slot 27 +0x6C IsActionReadyForInterruption).
// Checked: the ability looked up by interface type 9 over a two-node tree; the world seed fetched once before the validity test (from
//   the CTCCombat `this` still in ecx) and stepped only for an idle ability whose RepeatTimer is -1; the roll (rotated seed mod 100,
//   unsigned) against PercentageChance; the entity-events component found by the map lookup and GetEvent given the right component, the
//   out slot and a filter whose third predicate holds &hit_by; IsPerformingAnyAction and the action's interrupt test on the parent;
//   ActiveTimer = (long)(ConstantFPS * SecondsDuration) and FinishCurrentAction exactly when every test passes; nothing else written.
#include <stdio.h>
#include <string.h>

extern "C" long g_TestConstantFPS = 30;
#pragma comment(linker, "/alternatename:?ConstantFPS@CMainGameComponent@@1JA=_g_TestConstantFPS")

// The row, through a __fastcall stand-in for its __fastcall frame (ecx = this, edx unused, one stack argument popped by the callee).
extern "C" void __fastcall RowUpdateGetHitCounterAttack(void* self, int unused, const void* hit_by);
#pragma comment(linker, "/alternatename:@RowUpdateGetHitCounterAttack@12=?UpdateGetHitCounterAttack@CTCCombat@@QAEXABVCThingCreatureBase@@@Z")

static unsigned char g_combat[0x98], g_thing[0x200], g_world[16], g_events[16], g_action[16], g_hitby[16];
static unsigned char g_header[0x24], g_nodeA[0x24], g_nodeB[0x24], g_dataA[0x18], g_dataB[0x18];
static unsigned long g_seed;
static int g_peekWorld, g_getSeed, g_ror, g_getEvent, g_isPerforming, g_ready, g_finish, g_bad;
static bool g_eventResult, g_performing, g_readyResult;
struct TCEntry { long type; void* tc; };
static TCEntry g_components[3];

extern "C" void* __fastcall StubPeekWorld(void* self)
{
    ++g_peekWorld;
    if (self != g_combat) { printf("PeekWorld this %p\n", self); ++g_bad; }
    return g_world;
}
#pragma comment(linker, "/alternatename:?PeekWorld@CTCBase@@IBEABVCWorld@@XZ=@StubPeekWorld@4")
extern "C" unsigned long* __fastcall StubGetWorldSeed(void* self)
{
    ++g_getSeed;
    if (self != g_world) { printf("GetWorldSeed this %p\n", self); ++g_bad; }
    return &g_seed;
}
#pragma comment(linker, "/alternatename:?GetWorldSeed@CWorld@@QBEPAKXZ=@StubGetWorldSeed@4")
long __fastcall GFROR13(unsigned long value)
{
    ++g_ror;
    return (long)((value >> 13) | (value << 19));
}
// GetEvent<const CEventHitBy, CFilter_Bind3<..> > (0x00738A80): an explicit specialisation of a minimal declaration carries the
// instance's exact symbol (the filter holds its three predicates by reference: F3 -> CIsHitEventHitBy { const CThing* HitBy }).
namespace NEntityEvents { class CEventHitBy; class CIsHitEventHitByMelee; class CIsHitEventBlocked; class CIsHitEventHitBy; }
namespace std { template<class P> class unary_negate; }
template<class A, class B, class C> class CFilter_Bind3 { public: const A& F1; const B& F2; const C& F3; };
class CTCEntityEvents { public: template<class E, class F> bool GetEvent(E**, const F&) const; };
typedef CFilter_Bind3<NEntityEvents::CIsHitEventHitByMelee, std::unary_negate<NEntityEvents::CIsHitEventBlocked>, NEntityEvents::CIsHitEventHitBy> HitFilter;
template<> bool CTCEntityEvents::GetEvent<const NEntityEvents::CEventHitBy, HitFilter>(const NEntityEvents::CEventHitBy** out, const HitFilter& filter) const
{
    ++g_getEvent;
    if ((const void*)this != g_events) { printf("GetEvent this %p\n", (const void*)this); ++g_bad; }
    if (*(void* const*)&filter.F3 != g_hitby) { printf("GetEvent filter HitBy %p\n", *(void* const*)&filter.F3); ++g_bad; }
    if (g_eventResult)
        *out = (const NEntityEvents::CEventHitBy*)g_hitby;
    return g_eventResult;
}
extern "C" bool __fastcall StubIsPerformingAnyAction(void* self)
{
    ++g_isPerforming;
    if (self != g_thing) { printf("IsPerformingAnyAction this %p\n", self); ++g_bad; }
    return g_performing;
}
#pragma comment(linker, "/alternatename:?IsPerformingAnyAction@CThingCreatureBase@@QBE_NXZ=@StubIsPerformingAnyAction@4")
extern "C" void __fastcall StubFinishCurrentAction(void* self)
{
    ++g_finish;
    if (self != g_thing) { printf("FinishCurrentAction this %p\n", self); ++g_bad; }
}
#pragma comment(linker, "/alternatename:?FinishCurrentAction@CThingCreatureBase@@QAEXXZ=@StubFinishCurrentAction@4")
static bool __fastcall ActionReady(void* self)
{
    ++g_ready;
    if (self != g_action) { printf("IsActionReadyForInterruption this %p\n", self); ++g_bad; }
    return g_readyResult;
}
static void* g_actionVtable[40];

struct Case {
    const char* name;
    long typeA, typeB;              // interface types of the two abilities (the second, B, is in-order after A)
    long repeat, active;            // B's CCombatAbility
    long chance;                    // B's PercentageChance
    bool hasEvents, eventResult, performing, ready;
};

static void Node(unsigned char* n, unsigned char* parent, unsigned char* left, unsigned char* right, long key, unsigned char* data)
{
    *(void**)(n + 4) = parent; *(void**)(n + 8) = left; *(void**)(n + 0x0C) = right;
    *(long*)(n + 0x10) = key; *(void**)(n + 0x14) = data; *(void**)(n + 0x18) = 0;
}

static bool Run(const Case& c, unsigned long seed)
{
    memset(g_combat, 0x5A, sizeof g_combat); memset(g_thing, 0, sizeof g_thing);
    memset(g_header, 0, sizeof g_header); memset(g_dataA, 0, sizeof g_dataA); memset(g_dataB, 0, sizeof g_dataB);
    g_peekWorld = g_getSeed = g_ror = g_getEvent = g_isPerforming = g_ready = g_finish = g_bad = 0;
    g_eventResult = c.eventResult; g_performing = c.performing; g_readyResult = c.ready; g_seed = seed;
    // tree: root A, A.right = B; header.left = A (leftmost), header.right = B (rightmost), header.parent = A
    Node(g_nodeA, g_header, 0, g_nodeB, 1, g_dataA);
    Node(g_nodeB, g_nodeA, 0, 0, 2, g_dataB);
    *(void**)(g_header + 4) = g_nodeA; *(void**)(g_header + 8) = g_nodeA; *(void**)(g_header + 0x0C) = g_nodeB;
    *(long*)(g_nodeA + 0x1C) = -7; *(long*)(g_nodeA + 0x20) = 123;          // A: active, must not be taken unless its type is 9
    *(long*)(g_nodeB + 0x1C) = c.repeat; *(long*)(g_nodeB + 0x20) = c.active;
    *(long*)(g_dataA + 0x04) = 100; *(float*)(g_dataA + 0x08) = 9.0f; *(long*)(g_dataA + 0x14) = c.typeA;
    *(long*)(g_dataB + 0x04) = c.chance; *(float*)(g_dataB + 0x08) = 1.5f; *(float*)(g_dataB + 0x0C) = 7.0f; *(long*)(g_dataB + 0x14) = c.typeB;
    *(void**)(g_combat + 0x04) = g_thing;
    *(void**)(g_combat + 0x70) = g_header; *(unsigned long*)(g_combat + 0x74) = 2;
    if (c.hasEvents) g_thing[0x28] = 4;
    g_components[0].type = 0x10; g_components[0].tc = (void*)0x1111;
    g_components[1].type = 0x42; g_components[1].tc = g_events;
    g_components[2].type = 0x50; g_components[2].tc = (void*)0x2222;
    *(void**)(g_thing + 0x44) = &g_components[0]; *(void**)(g_thing + 0x48) = &g_components[3];
    for (int i = 0; i < 40; ++i) g_actionVtable[i] = 0;
    g_actionVtable[27] = (void*)ActionReady;
    *(void**)g_action = g_actionVtable;
    *(void**)(g_thing + 0x100) = g_action;

    unsigned char before[0x98]; memcpy(before, g_combat, sizeof before);
    unsigned char nodeA0[0x24], nodeB0[0x24]; memcpy(nodeA0, g_nodeA, sizeof nodeA0); memcpy(nodeB0, g_nodeB, sizeof nodeB0);
    RowUpdateGetHitCounterAttack(g_combat, 0, g_hitby);

    // model
    unsigned char* found = c.typeA == 9 ? g_nodeA : (c.typeB == 9 ? g_nodeB : 0);
    int wantWorld = found ? 1 : 0, wantRor = 0, wantEvent = 0, wantPerf = 0, wantReady = 0, wantFinish = 0;
    unsigned long wantSeed = seed;
    long wantActive = *(long*)(nodeB0 + 0x20), wantActiveA = *(long*)(nodeA0 + 0x20);
    if (found) {
        const unsigned char* snap = found == g_nodeA ? nodeA0 : nodeB0;   // the node as it was before the call
        long rep = *(long*)(snap + 0x1C), act = *(long*)(snap + 0x20);
        long chance = found == g_nodeA ? 100 : c.chance;
        bool valid = act >= 0;
        if (!valid && rep == -1) {
            unsigned long s = seed * 0x24a1 + 0x24df; s = (s >> 13) | (s << 19); wantSeed = s; wantRor = 1;
            valid = s % 100 < (unsigned long)chance;
        }
        if (valid && c.hasEvents) {
            wantEvent = 1;
            if (c.eventResult) {
                wantPerf = 1;
                if (c.performing) {
                    wantReady = 1;
                    if (c.ready) {
                        wantFinish = 1;
                        if (found == g_nodeB) wantActive = (long)(30 * 1.5f); else wantActiveA = (long)(30 * 9.0f);
                    }
                }
            }
        }
    }
    bool ok = g_bad == 0;
    if (g_peekWorld != wantWorld || g_getSeed != wantWorld || g_ror != wantRor || g_getEvent != wantEvent || g_isPerforming != wantPerf
        || g_ready != wantReady || g_finish != wantFinish) {
        printf("calls world %d/%d seed %d ror %d/%d event %d/%d perf %d/%d ready %d/%d finish %d/%d\n", g_peekWorld, wantWorld, g_getSeed,
               g_ror, wantRor, g_getEvent, wantEvent, g_isPerforming, wantPerf, g_ready, wantReady, g_finish, wantFinish);
        ok = false;
    }
    if (g_seed != wantSeed) { printf("seed %08lx want %08lx\n", g_seed, wantSeed); ok = false; }
    if (*(long*)(g_nodeB + 0x20) != wantActive) { printf("B ActiveTimer %ld want %ld\n", *(long*)(g_nodeB + 0x20), wantActive); ok = false; }
    if (*(long*)(g_nodeA + 0x20) != wantActiveA) { printf("A ActiveTimer %ld want %ld\n", *(long*)(g_nodeA + 0x20), wantActiveA); ok = false; }
    if (memcmp(g_nodeB, nodeB0, 0x20) || memcmp(g_nodeA, nodeA0, 0x20) || memcmp(before, g_combat, sizeof before)) {
        printf("other bytes written\n"); ok = false;
    }
    printf("%-34s seed %08lx: %s\n", c.name, seed, ok ? "ok" : "FAIL");
    return ok;
}

int main()
{
    // the rolls (rotated seed mod 100) are computed by the model; chances 18 / 19 straddle seed 1's roll
    static const Case cases[] = {
        { "no type-9 ability",              3, 4, -1, -1, 100, true, true, true, true },
        { "active, no events component",    3, 9,  5, 10,   0, false, true, true, true },
        { "active, all pass",               3, 9,  5, 10,   0, true, true, true, true },
        { "active with timer 0, all pass",  3, 9,  5,  0,   0, true, true, true, true },
        { "idle, repeat timer running",     3, 9,  4, -1, 100, true, true, true, true },
        { "idle, roll wins",                3, 9, -1, -1, 100, true, true, true, true },
        { "idle, roll loses",               3, 9, -1, -1,   0, true, true, true, true },
        { "idle, roll vs chance 19",        3, 9, -1, -1,  19, true, true, true, true },
        { "idle, roll vs chance 18",        3, 9, -1, -1,  18, true, true, true, true },
        { "no matching event",              3, 9, -1, -1, 100, true, false, true, true },
        { "not performing an action",       3, 9, -1, -1, 100, true, true, false, true },
        { "action not interruptible",       3, 9, -1, -1, 100, true, true, true, false },
        { "first node is type 9 (active)",  9, 9, -1, -1, 100, true, true, true, true },
        { "negative chance (unsigned roll)",3, 9, -1, -1,  -5, true, true, true, true },
    };
    bool ok = true;
    for (unsigned i = 0; i < sizeof cases / sizeof cases[0]; ++i) {
        ok &= Run(cases[i], 1);
        ok &= Run(cases[i], 7);
        ok &= Run(cases[i], 0xDEADBEEF);
    }
    puts(ok ? "00733cf0_TEST PASS" : "00733cf0_TEST FAIL");
    return ok ? 0 : 1;
}