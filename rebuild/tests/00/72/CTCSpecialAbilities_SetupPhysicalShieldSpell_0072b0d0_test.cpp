#include <cstdio>
// Behaviour model of CTCSpecialAbilities::SetupPhysicalShieldSpell (0x0072B0D0): an existing shield is cancelled when active
// and the call returns false; without one a component is added and the call returns true.
struct Shield { bool active; int cancels; };
struct Creature { bool hasShield; Shield shield; int added; };
static bool Setup(Creature& c)
{
    if (c.hasShield) {
        if (c.shield.active) c.shield.cancels++;
        return false;
    }
    c.added++;
    return true;
}
int main()
{
    Creature a = { true, { true, 0 }, 0 }, b = { true, { false, 0 }, 0 }, n = { false, { false, 0 }, 0 };
    bool ra = Setup(a), rb = Setup(b), rn = Setup(n);
    if (ra || rb || !rn || a.shield.cancels != 1 || b.shield.cancels || n.added != 1 || a.added) { std::printf("0072b0d0_TEST FAIL\n"); return 1; }
    std::printf("0072b0d0_TEST PASS\n");
    return 0;
}