#include <cstdio>
struct V { float x, y, z; };
struct Phys { V pos; };
struct Thing { Phys* PhysicsTC; };
struct F { V Pos; float MinRange; float MaxRange; };
static bool In2D(const V& a, const V& b, float r) { float dx = a.x - b.x, dy = a.y - b.y; return dx * dx + dy * dy < r * r; }
static bool Run(const F& f, Thing* t)
{
    Phys* p = t->PhysicsTC;
    if (p) return !In2D(f.Pos, p->pos, f.MinRange) && In2D(f.Pos, p->pos, f.MaxRange);
    return false;
}
int main()
{
    F f = { {0, 0, 0}, 2.0f, 5.0f };
    Phys p = { {3, 0, 100} };
    Thing t = { &p };
    Thing none = { 0 };
    bool ok = Run(f, &t) && !Run(f, &none);
    p.pos.x = 1; ok = ok && !Run(f, &t);
    p.pos.x = 6; ok = ok && !Run(f, &t);
    p.pos.x = 2; ok = ok && Run(f, &t);
    if (!ok) { std::printf("006a7700_TEST FAIL\n"); return 1; }
    std::printf("006a7700_TEST PASS\n");
    return 0;
}
