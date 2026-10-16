#include <cstdio>
// Behaviour model of CTCSpecialAbilities::vfunc_16 (0x0072C3C0): when switched on, every follower thing that exists, is not
// dead and is not unconscious is killed as reclaimed undead.
struct Thing { bool dead; bool unconscious; };
int main()
{
    Thing a = { false, false }, b = { true, false }, c = { false, true };
    Thing* followers[4] = { &a, &b, &c, 0 };
    int killed[2] = { 0, 0 };
    for (int pass = 0; pass < 2; ++pass) {
        bool on = pass == 0;
        if (on)
            for (int i = 0; i < 4; ++i) {
                Thing* t = followers[i];
                if (t && !t->dead && !t->unconscious) killed[pass]++;
            }
    }
    if (killed[0] != 1 || killed[1] != 0) { std::printf("0072c3c0_TEST FAIL\n"); return 1; }
    std::printf("0072c3c0_TEST PASS\n");
    return 0;
}