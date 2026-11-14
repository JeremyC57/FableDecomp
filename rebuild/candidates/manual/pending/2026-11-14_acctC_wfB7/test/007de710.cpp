// Behaviour test for CTCWallMount::AttachObject (retail 0x007DE710): the trophy pointer is stored; with BOTH the mount's wall dummy and
// the object's weapon-focus dummy found the object is moved so the focus dummy sits on the wall dummy (object * inverse(focus) * wall,
// the inverse being the uniform-scale one: transpose over the squared scale); then, always: physics off, out of limbo, particles
// detached, not targetable, a living hero's mounted-trophy count bumped, the bought-for amount stored, mount and trophy flagged as
// game-state persisted, the wall-mount effects told. Returns true.
#include <cstdio>
#include <cmath>
struct M { float r[3][3]; float t[3]; };
static M Mul(const M& a, const M& b)
{
    M c;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j) c.r[i][j] = a.r[i][0] * b.r[0][j] + a.r[i][1] * b.r[1][j] + a.r[i][2] * b.r[2][j];
    for (int j = 0; j < 3; ++j) c.t[j] = a.t[0] * b.r[0][j] + a.t[1] * b.r[1][j] + a.t[2] * b.r[2][j] + b.t[j];
    return c;
}
static M InverseUSRT(const M& m)
{
    M v;
    float scale_squared = m.r[0][0] * m.r[0][0] + m.r[0][1] * m.r[0][1] + m.r[0][2] * m.r[0][2];
    float scale = 1.0f / scale_squared;
    for (int i = 0; i < 3; ++i) for (int j = 0; j < 3; ++j) v.r[i][j] = m.r[j][i] * scale;
    for (int j = 0; j < 3; ++j) v.t[j] = -(m.t[0] * v.r[0][j] + m.t[1] * v.r[1][j] + m.t[2] * v.r[2][j]);
    return v;
}
static M Scaled(float s, float x, float y, float z) { M m = { { { s, 0, 0 }, { 0, s, 0 }, { 0, 0, s } }, { x, y, z } }; return m; }
struct Trophy { M world; bool physics, limbo, particles, targetable, persisted, effects_told; };
struct Hero { bool alive; int trophies; };
struct Mount { long bought; const Trophy* trophy; bool persisted; };
static bool Attach(Mount& mount, Trophy& object, long amount, bool wall_found, const M& wall, bool focus_found, const M& focus, Hero* hero)
{
    mount.trophy = &object;
    bool attach = true;
    if (!wall_found) attach = false;
    if (!focus_found) attach = false;
    if (attach) object.world = Mul(Mul(object.world, InverseUSRT(focus)), wall);
    object.physics = false;
    object.limbo = false;
    object.particles = false;
    object.targetable = false;
    if (hero && hero->alive) ++hero->trophies;
    mount.bought = amount;
    mount.persisted = true;
    object.persisted = true;
    object.effects_told = true;
    return true;
}
static Trophy NewTrophy() { Trophy t = { Scaled(1, 1, 2, 3), true, true, true, true, false, false }; return t; }
int main()
{
    Mount mount = { 0, 0, false }; Hero hero = { true, 2 };
    M wall = Scaled(1, 10, 0, 5), focus = Scaled(2, 0, 0, 1);
    Trophy a = NewTrophy();
    // (1,2,3) * inverse(scale 2 at (0,0,1)) = 0.5 * (1,2,3) - (0,0,0.5) = (0.5, 1, 1); then + (10, 0, 5)
    bool ok = Attach(mount, a, 300, true, wall, true, focus, &hero)
        && std::fabs(a.world.t[0] - 10.5f) < 1e-4 && std::fabs(a.world.t[1] - 1) < 1e-4 && std::fabs(a.world.t[2] - 6) < 1e-4
        && std::fabs(a.world.r[0][0] - 0.5f) < 1e-4 && !a.physics && !a.limbo && !a.particles && !a.targetable && a.persisted
        && a.effects_told && hero.trophies == 3 && mount.bought == 300 && mount.trophy == &a && mount.persisted;
    Trophy b = NewTrophy();                                              // no wall dummy: not moved, the rest still happens
    ok = ok && Attach(mount, b, 50, false, wall, true, focus, &hero) && b.world.t[0] == 1 && b.world.r[0][0] == 1 && !b.physics
        && hero.trophies == 4 && mount.bought == 50 && mount.trophy == &b;
    Trophy c = NewTrophy();                                              // no focus dummy: not moved either
    ok = ok && Attach(mount, c, 60, true, wall, false, focus, &hero) && c.world.t[2] == 3 && hero.trophies == 5;
    Trophy d = NewTrophy(); Hero dead = { false, 7 };                    // a dead hero's count is left alone
    ok = ok && Attach(mount, d, 70, true, wall, true, focus, &dead) && dead.trophies == 7 && mount.bought == 70
        && Attach(mount, d, 80, true, wall, true, focus, 0) && mount.bought == 80;
    if (!ok) { std::printf("007de710_TEST FAIL\n"); return 1; }
    std::printf("007de710_TEST PASS\n");
    return 0;
}
