// Behaviour test for 0x00857520 CCombatActionBase::Initialise.
#include <cstdio>
struct CAnimationEntry { int id; };
struct CTCCombat { void* chained; void SetNextComboChainPos(void* a) { chained = a; } };
struct CCreature {
    long pick; bool hasCombat; CTCCombat combat;
    long GetAppropriateCombatAnimationFromList(const CAnimationEntry* const*) { return pick; }
    bool GetInterface(CTCCombat** out, long type) { if (type == 0x30 && hasCombat) { *out = &combat; return true; } return false; }
};
static int g_reset = 0, g_baseInit = 0;
struct CCombatActionBase {
    CCreature* creature; const CAnimationEntry* anims[3]; const CAnimationEntry* played; float blend, speed; bool strike;
    void ResetCombatState() { ++g_reset; }
    void SetAnimation(const CAnimationEntry* e, float b, float s) { played = e; blend = b; speed = s; }
    void BaseInitialise() { ++g_baseInit; }
    bool IsStrikeAction() { return strike; }
    void Initialise()
    {
        ResetCombatState();
        long index = creature->GetAppropriateCombatAnimationFromList(anims);
        SetAnimation(*(anims + index), -1.0f, 1.0f);
        BaseInitialise();
        if (IsStrikeAction()) {
            CTCCombat* pCombat;
            if (creature->GetInterface(&pCombat, 0x30)) pCombat->SetNextComboChainPos(this);
        }
    }
};
int main()
{
    CAnimationEntry e0 = { 0 }, e1 = { 1 }, e2 = { 2 };
    CCreature cr; cr.pick = 2; cr.hasCombat = true; cr.combat.chained = 0;
    CCombatActionBase a; a.creature = &cr; a.anims[0] = &e0; a.anims[1] = &e1; a.anims[2] = &e2; a.strike = false;
    a.Initialise();
    if (g_reset != 1 || g_baseInit != 1 || a.played != &e2 || a.blend != -1.0f || a.speed != 1.0f || cr.combat.chained != 0) {
        std::printf("00857520_TEST FAIL\n"); return 1;
    }
    a.strike = true; cr.pick = 1; a.Initialise();
    if (a.played != &e1 || cr.combat.chained != &a) { std::printf("00857520_TEST FAIL\n"); return 1; }
    std::printf("00857520_TEST PASS\n");
    return 0;
}