#include <cstdio>
// Behaviour model of CTCStealth::IsAvailableToPerformStealthAttack (0x00747790): needs a carrying component, a live weapon in the
// primary slot that has a weapon component, and that weapon's verdict.
struct Weapon { bool dead; bool hasWeaponTC; bool canStealth; };
struct Owner { bool hasCarrying; Weapon* primary; };
static bool Available(const Owner& o)
{
    if (o.hasCarrying) {
        Weapon* w = o.primary;
        if (w && !w->dead && w->hasWeaponTC) return w->canStealth;
    }
    return false;
}
int main()
{
    Weapon good = { false, true, true }, dead = { true, true, true }, plain = { false, false, true }, no = { false, true, false };
    Owner a = { true, &good }, b = { true, &dead }, c = { true, &plain }, d = { true, &no }, e = { false, &good }, f = { true, 0 };
    if (!Available(a) || Available(b) || Available(c) || Available(d) || Available(e) || Available(f)) { std::printf("00747790_TEST FAIL\n"); return 1; }
    std::printf("00747790_TEST PASS\n");
    return 0;
}
