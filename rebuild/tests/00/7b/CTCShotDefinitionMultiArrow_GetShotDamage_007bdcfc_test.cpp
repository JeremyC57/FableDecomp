#include <cstdio>
// Behaviour model of CTCShotDefinitionMultiArrow::GetShotDamage (0x007BDCFC): base damage times the shooter's multiplier.
struct Shooter { bool alive; bool hasWrath; float wrath; };
static float Damage(const Shooter* s, float base)
{
    float m = 1.0f;
    if (s && s->alive && s->hasWrath) m = s->wrath;
    return base * m;
}
int main()
{
    Shooter a = { true, true, 2.0f }, b = { true, false, 2.0f }, c = { false, true, 2.0f };
    if (Damage(&a, 5.0f) != 10.0f || Damage(&b, 5.0f) != 5.0f || Damage(&c, 5.0f) != 5.0f || Damage(0, 3.0f) != 3.0f) { std::printf("007bdcfc_TEST FAIL\n"); return 1; }
    std::printf("007bdcfc_TEST PASS\n");
    return 0;
}