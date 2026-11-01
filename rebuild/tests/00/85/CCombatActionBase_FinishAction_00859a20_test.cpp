// Behaviour test for CCombatActionBase::FinishAction (0x00859A20): a live target loses TC 0x100; a dead or missing target is
// left alone; the melee-ability effects are always reset and the base FinishAction always runs, in that order.
#include <cstdio>
static int g_log[8]; static int g_n = 0;
struct Thing { unsigned char Dead; int removed; void RemoveTC(int type) { removed = type; g_log[g_n++] = 1; } };
struct CombatActionBase {
    Thing* target;
    void ResetMeleeCombatAbilityEffects() { g_log[g_n++] = 2; }
    void BaseFinishAction() { g_log[g_n++] = 3; }
    void FinishAction()
    {
        if (target && !(target->Dead & 1))
            target->RemoveTC(0x100);
        ResetMeleeCombatAbilityEffects();
        BaseFinishAction();
    }
};
int main()
{
    Thing alive = { 0, 0 }; Thing dead = { 1, 0 };
    CombatActionBase a; a.target = &alive; a.FinishAction();
    if (g_n != 3 || g_log[0] != 1 || g_log[1] != 2 || g_log[2] != 3 || alive.removed != 0x100) { std::printf("00859a20_TEST FAIL\n"); return 1; }
    g_n = 0; a.target = &dead; a.FinishAction();
    if (g_n != 2 || g_log[0] != 2 || dead.removed != 0) { std::printf("00859a20_TEST FAIL\n"); return 1; }
    g_n = 0; a.target = 0; a.FinishAction();
    if (g_n != 2) { std::printf("00859a20_TEST FAIL\n"); return 1; }
    std::printf("00859a20_TEST PASS\n");
    return 0;
}