// Test: CAIStateGroup_MeleeCombatNearby2::IsCombatSequenceActionPending -- performing any action, else the combat manager's non-interruptable sequence test
#include <cstdio>
struct CCreature { bool performing; bool inSequence; };
static int g_managerCalls;
static bool IsCreaturePerformingNonInterruptableSequence(const CCreature& c) { ++g_managerCalls; return c.inSequence; }
static bool IsPending(const CCreature* c)
{
    if (c->performing) return true;
    return IsCreaturePerformingNonInterruptableSequence(*c);
}
int main()
{
    CCreature a = { true, false }, b = { false, true }, c = { false, false };
    if (!IsPending(&a) || g_managerCalls != 0 || !IsPending(&b) || IsPending(&c) || g_managerCalls != 2) { std::printf("0093dae0_TEST FAIL\n"); return 1; }
    std::printf("0093dae0_TEST PASS\n");
    return 0;
}
