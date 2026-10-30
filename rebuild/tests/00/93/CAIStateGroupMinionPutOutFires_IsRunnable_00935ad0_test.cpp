// behaviour test for 0x00935AD0 CAIStateGroup_MinionPutOutFires::IsRunnable: needs perception; clears the target, picks the
// nearest burning thing within the extended sight radius (2D), latches it, runnable when it is alive
#include <cstdio>
#include <cmath>
struct Thing { float x, y; bool burning; unsigned char Dead : 1; bool IsDead() const { return Dead; } };
struct Creature { float x, y; bool perceives; float sight; };
static bool IsRunnable(const Creature& c, Thing* things, int n, Thing*& target)
{
    if (!c.perceives) return false;
    target = 0;
    Thing* best = 0; float bestD = 0;
    for (int i = 0; i < n; ++i) {
        float dx = things[i].x - c.x, dy = things[i].y - c.y, d = std::sqrt(dx * dx + dy * dy);
        if (!things[i].burning || d > c.sight) continue;
        if (!best || d < bestD) { best = &things[i]; bestD = d; }
    }
    if (!best) return false;
    target = best;
    return target != 0 && !target->IsDead();
}
int main()
{
    Thing t[3] = { { 5, 0, true, 0 }, { 2, 0, true, 0 }, { 1, 0, false, 0 } };
    Creature c = { 0, 0, true, 10 };
    Thing* tg = &t[2];
    bool ok = IsRunnable(c, t, 3, tg) && tg == &t[1];
    t[1].Dead = 1;
    ok = ok && !IsRunnable(c, t, 3, tg) && tg == &t[1];
    c.sight = 1.5f; tg = &t[0];
    ok = ok && !IsRunnable(c, t, 3, tg) && tg == 0;
    c.perceives = false; tg = &t[0];
    ok = ok && !IsRunnable(c, t, 3, tg) && tg == &t[0];
    std::printf(ok ? "00935ad0_TEST PASS\n" : "00935ad0_TEST FAIL\n");
    return ok ? 0 : 1;
}