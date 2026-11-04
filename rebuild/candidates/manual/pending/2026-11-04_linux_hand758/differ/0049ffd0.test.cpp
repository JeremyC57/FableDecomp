// Behaviour test for CWorld::SetKickPos (0x0049FFD0), g++ model: nearest thing within 30 (2D) carrying TC 0x63 or 0xDD sets the kick position, angle and the 0xDD flag.
#include <cstdio>
#include <cmath>
#include <vector>
struct Thing { float x, y, z, angle; bool tc63, tcDD; };
struct Kick { bool set; float x, y, z, angle; bool dd; };
static void SetKickPos(const Thing& hero, bool heroHasStats, const std::vector<Thing>& things, Kick& out)
{
    out.set = false;
    if (!heroHasStats) return;
    const Thing* best = 0; float bestD = 0;
    for (size_t i = 0; i < things.size(); ++i) {
        const Thing& t = things[i];
        if (!(t.tc63 || t.tcDD)) continue;
        float dx = t.x - hero.x, dy = t.y - hero.y, d2 = dx * dx + dy * dy;
        if (d2 > 30.0f * 30.0f) continue;
        float d3 = d2 + (t.z - hero.z) * (t.z - hero.z);
        if (!best || d3 < bestD) { best = &t; bestD = d3; }
    }
    if (best) { out.set = true; out.x = best->x; out.y = best->y; out.z = best->z; out.angle = best->angle; out.dd = best->tcDD; }
}
int main()
{
    Thing hero = { 0, 0, 0, 0, false, false };
    std::vector<Thing> t;
    Thing a = { 10, 0, 0, 1.5f, true, false }, b = { 5, 5, 0, 0.5f, false, true }, c = { 2, 0, 0, 0, false, false }, far = { 40, 0, 0, 0, true, true };
    t.push_back(a); t.push_back(b); t.push_back(c); t.push_back(far);
    Kick k; SetKickPos(hero, true, t, k);
    int ok = k.set && k.x == 5 && k.y == 5 && k.dd && k.angle == 0.5f;
    Kick k2; SetKickPos(hero, false, t, k2); ok = ok && !k2.set;
    std::vector<Thing> none(1, far); Kick k3; SetKickPos(hero, true, none, k3); ok = ok && !k3.set;
    std::printf(ok ? "0049ffd0_TEST PASS\n" : "0049ffd0_TEST FAIL\n");
    return ok ? 0 : 1;
}
