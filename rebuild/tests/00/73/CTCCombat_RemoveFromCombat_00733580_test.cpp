#include <cstdio>

static int g_reset = 0; static void* g_dereg = 0;
struct Cr { bool reg; bool IsRegisteredCombatant() { return reg; } };
struct Mg { void DeregisterCombatant(Cr* c) { g_dereg = c; } };
struct C { unsigned char pad[0x20]; Mg* CombatManager; Cr* ParentCreature;
    void ResetCombatState() { ++g_reset; }
    void RemoveFromCombat() { ResetCombatState(); if (ParentCreature->IsRegisteredCombatant()) CombatManager->DeregisterCombatant(ParentCreature); } };
int main()
{
    Cr cr; Mg mg; C c; c.CombatManager = &mg; c.ParentCreature = &cr; cr.reg = false; c.RemoveFromCombat();
    if (g_reset != 1 || g_dereg) { std::printf("00733580_TEST FAIL\n"); return 1; }
    cr.reg = true; c.RemoveFromCombat(); if (g_reset != 2 || g_dereg != &cr) { std::printf("00733580_TEST FAIL\n"); return 1; }
    std::printf("00733580_TEST PASS\n");
    return 0;
}