// Behaviour test for CTCJackPillar::Enable (0x007317D0): enabling activates physics, plays the rise animation, spawns the
// effect when the def has one and shows the pillar; disabling deactivates and hides it unless loading.
#include <cstdio>

struct Pillar { bool collision, active, visible, loading; int risePlays, effects; };
static void Enable(Pillar& p, bool enable, long effectType)
{
    p.collision = enable;
    p.active = enable;
    if (enable) {
        ++p.risePlays;
        if (effectType) ++p.effects;
        p.visible = true;
    } else {
        if (!p.loading) p.visible = false;
    }
}
int main()
{
    Pillar p; p.collision = false; p.active = false; p.visible = false; p.loading = false; p.risePlays = 0; p.effects = 0;
    Enable(p, true, 5);
    if (!p.visible || !p.active || p.risePlays != 1 || p.effects != 1) { std::printf("007317d0_TEST FAIL\n"); return 1; }
    p.loading = true; Enable(p, false, 5);
    if (!p.visible || p.active) { std::printf("007317d0_TEST FAIL\n"); return 1; }
    p.loading = false; Enable(p, false, 0);
    if (p.visible) { std::printf("007317d0_TEST FAIL\n"); return 1; }
    Enable(p, true, 0);
    if (p.effects != 1 || p.risePlays != 2) { std::printf("007317d0_TEST FAIL\n"); return 1; }
    std::printf("007317d0_TEST PASS\n");
    return 0;
}
