#include <cstdio>
// Behaviour model of CTCMultiArrow::RemoveEffectOnWeapon (0x007CC120).
struct Weapon { bool dead; bool hasAppearance; int cleared; };
struct Arrow { bool removed; };
static void RemoveEffect(Arrow& a, Weapon* w)
{
    if (!a.removed) {
        if (w && !w->dead && w->hasAppearance) w->cleared += 2;
        a.removed = true;
    }
}
int main()
{
    Weapon w = { false, true, 0 };
    Arrow a = { false };
    RemoveEffect(a, &w); RemoveEffect(a, &w);
    if (!a.removed || w.cleared != 2) { std::printf("007cc120_TEST FAIL\n"); return 1; }
    Arrow b = { false };
    RemoveEffect(b, 0);
    if (!b.removed) { std::printf("007cc120_TEST FAIL\n"); return 1; }
    std::printf("007cc120_TEST PASS\n");
    return 0;
}
