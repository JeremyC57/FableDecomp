#include <cstdio>
// Behaviour model of CTCHearabilityManager::GetSoundRadiusWithModifiers (0x007475C0).
struct Exp { float mult; };
struct Src { float radius; };
static float Get(const Exp* e, const Src* s)
{
    float m = 1.0f;
    if (e) m = e->mult;
    if (s) return s->radius * m;
    return m;
}
int main()
{
    Exp e; e.mult = 0.5f; Src s; s.radius = 10.0f;
    if (Get(&e, &s) != 5.0f || Get(0, &s) != 10.0f || Get(&e, 0) != 0.5f || Get(0, 0) != 1.0f) { std::printf("007475c0_TEST FAIL\n"); return 1; }
    std::printf("007475c0_TEST PASS\n");
    return 0;
}