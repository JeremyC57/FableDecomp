#include <cstdio>
// Behaviour model of CTCMultiStrike::RemoveEffectOnWeapon (0x007CC9E0).
struct Thing { bool dead; int kills; };
static void RemoveEffect(Thing* weapon, Thing** effects, long n)
{
    if (weapon && !weapon->dead)
        for (long i = 0; i < n; ++i)
            if (effects[i] && !effects[i]->dead) effects[i]->kills++;
}
int main()
{
    Thing w = { false, 0 }, dw = { true, 0 }, a = { false, 0 }, b = { true, 0 };
    Thing* fx[3] = { &a, &b, 0 };
    RemoveEffect(&dw, fx, 3);
    if (a.kills) { std::printf("007cc9e0_TEST FAIL\n"); return 1; }
    RemoveEffect(&w, fx, 3);
    if (a.kills != 1 || b.kills) { std::printf("007cc9e0_TEST FAIL\n"); return 1; }
    std::printf("007cc9e0_TEST PASS\n");
    return 0;
}