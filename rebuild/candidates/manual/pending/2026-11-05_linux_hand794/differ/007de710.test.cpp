// Behaviour test for CTCWallMount::AttachObject (0x007DE710): with both dummies found the trophy is moved so its focus point lands on the
// wall dummy (object * inverse(focus) * wall, here pure translations), physics off, out of limbo, particles detached, not targetable,
// the hero's trophy count bumped, the bought-for amount stored; without a wall dummy the transform is skipped but the rest still runs.
#include <cstdio>
struct M { float t[3]; };
static M Mul(const M& a, const M& b) { M r; for (int i = 0; i < 3; ++i) r.t[i] = a.t[i] + b.t[i]; return r; }
static M Inv(const M& a) { M r; for (int i = 0; i < 3; ++i) r.t[i] = -a.t[i]; return r; }
struct Trophy { M pos; bool physics, limbo, particles, targetable; };
struct Mount { long bought; int heroTrophies; };
static bool Attach(Mount& w, Trophy& o, long amount, bool wallDummy, bool focusDummy, const M& wall, const M& focus)
{
    if (wallDummy && focusDummy) o.pos = Mul(Mul(o.pos, Inv(focus)), wall);
    o.physics = false; o.limbo = false; o.particles = false; o.targetable = false;
    ++w.heroTrophies; w.bought = amount;
    return true;
}
int main()
{
    Mount w = { 0, 2 }; Trophy o = { { { 1, 2, 3 } }, true, true, true, true };
    M wall = { { 10, 0, 5 } }, focus = { { 0.5f, 0, 0 } };
    bool ok = Attach(w, o, 300, true, true, wall, focus) && o.pos.t[0] == 10.5f && o.pos.t[1] == 2 && o.pos.t[2] == 8
        && !o.physics && !o.limbo && !o.particles && !o.targetable && w.heroTrophies == 3 && w.bought == 300;
    Trophy p = { { { 1, 1, 1 } }, true, true, true, true };
    ok = ok && Attach(w, p, 50, false, true, wall, focus) && p.pos.t[0] == 1 && w.heroTrophies == 4 && w.bought == 50 && !p.physics;
    if (!ok) { std::printf("007de710_TEST FAIL\n"); return 1; }
    std::printf("007de710_TEST PASS\n");
    return 0;
}
