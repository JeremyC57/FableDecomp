// Behaviour test for 0x00E3B320 (CM_HeroDownstairs::Main): per-frame proximity check clearing two parent flags until aborted.
#include <cstdio>
struct Thing { float x; };
static bool Near(const Thing* a, const Thing* b, float d) { float dx = a->x - b->x; return dx * dx < d * d; }
struct Quest { Thing hero; int ticks; int abortAt; const Thing* GetHero() { return &hero; } void Tick() { ++ticks; hero.x -= 1.0f; } };
struct Parent { bool f48, f4a; };
struct Script {
    Quest* PQuest; Thing T; Parent* ParentClass;
    bool IsDone() { return PQuest->ticks >= PQuest->abortAt; }
    void Main()
    {
        while (!IsDone()) {
            if (Near(&T, PQuest->GetHero(), 3.0f)) {
                if (IsDone()) return;
                if (ParentClass->f48 != false) { if (IsDone()) return; ParentClass->f48 = false; }
                if (ParentClass->f4a != false) { if (IsDone()) return; ParentClass->f4a = false; }
            }
            PQuest->Tick();
        }
    }
};
int main()
{
    Quest q; q.hero.x = 10.0f; q.ticks = 0; q.abortAt = 5;
    Parent p = { true, true };
    Script s; s.PQuest = &q; s.T.x = 0.0f; s.ParentClass = &p;
    s.Main();
    int ok = q.ticks == 5 && p.f48 && p.f4a;            // hero reaches x=5: never within 3
    q.ticks = 0; q.abortAt = 10; q.hero.x = 10.0f; s.Main();
    ok &= q.ticks == 10 && !p.f48 && !p.f4a;            // hero at x=2 by tick 8
    if (!ok) { std::printf("00e3b320_TEST FAIL\n"); return 1; }
    std::printf("00e3b320_TEST PASS\n");
    return 0;
}