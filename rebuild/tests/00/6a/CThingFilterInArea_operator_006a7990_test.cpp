#include <cstdio>
struct V { float x, y, z; };
struct Phys { V pos; };
struct Thing { Phys* PhysicsTC; };
struct Box { float TLX, TLY, BRX, BRY;
    bool Contains(float x, float y) const { return x >= TLX && x < BRX && y >= TLY && y < BRY; } };
static bool Run(const Box& b, Thing* t) { Phys* p = t->PhysicsTC; if (p) return b.Contains(p->pos.x, p->pos.y); return false; }
int main()
{
    Box b = { 0, 0, 10, 10 };
    Phys p = { {0, 0, 5} };
    Thing t = { &p }, none = { 0 };
    bool ok = Run(b, &t) && !Run(b, &none);
    p.pos.x = 10; ok = ok && !Run(b, &t);
    p.pos.x = 5; p.pos.y = -1; ok = ok && !Run(b, &t);
    p.pos.y = 9.9f; ok = ok && Run(b, &t);
    if (!ok) { std::printf("006a7990_TEST FAIL\n"); return 1; }
    std::printf("006a7990_TEST PASS\n");
    return 0;
}