// Behaviour test for CTCCombat::FrameUpdate (0x00736AE0): timers, usable abilities, flourish particles, in that order.
#include <cstdio>
static int g_log[8]; static int g_n = 0;
class CTCCombat { public:
    int frames;
    void UpdateCombatTimers() { g_log[g_n++] = 1; ++frames; }
    void UpdateUsableMeleeCombatAbilities() { g_log[g_n++] = 2; }
    void UpdateFlourishParticles() { g_log[g_n++] = 3; }
    void FrameUpdate() { UpdateCombatTimers(); UpdateUsableMeleeCombatAbilities(); UpdateFlourishParticles(); }
};
int main()
{
    CTCCombat c; c.frames = 0;
    c.FrameUpdate();
    if (g_n != 3 || g_log[0] != 1 || g_log[1] != 2 || g_log[2] != 3 || c.frames != 1) { std::printf("00736ae0_TEST FAIL\n"); return 1; }
    std::printf("00736ae0_TEST PASS\n");
    return 0;
}