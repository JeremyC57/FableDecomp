#include <cstdio>
#include <cmath>
// Model of CThingPlayerCreature::SetActionForMelee's direction choice: the facing vector by default; Z-targeting a live target
// aims at it (flattened, normalised when longer than 1e-4); otherwise a non-zero last movement acceleration is rotated by the
// relative movement angle. An unusable carried weapon forces the flattened facing vector; a live best enemy is aimed at; zone 2
// unless the enemy is a creature (defender zone). The result is always true.
struct V { float X, Y, Z; };
static const float D = 1e-4f;
static V Flat(V v) { v.Z = 0.0f; if (v.X * v.X + v.Y * v.Y > D * D) { float m = std::sqrt(v.X * v.X + v.Y * v.Y); v.X /= m; v.Y /= m; } return v; }
static V Rotate(V v, float a) { V r = { v.X * std::cos(a) - v.Y * std::sin(a), v.X * std::sin(a) + v.Y * std::cos(a), v.Z }; return r; }
static V Choose(V facing, bool ztarget, bool target_alive, V to_target, V last, float angle, bool weapon_unusable, bool enemy_alive, V to_enemy)
{
    V dir = facing;
    if (ztarget && target_alive) dir = Flat(to_target);
    else if (!ztarget && last.X * last.X + last.Z * last.Z + last.Y * last.Y > D * D) dir = Rotate(last, angle);
    if (weapon_unusable) return Flat(facing);
    if (enemy_alive) return Flat(to_enemy);
    return dir;
}
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
int main()
{
    V f = { 0, 1, 0.5f }, t = { 3, 4, 7 }, z = { 0, 0, 0 }, l = { 1, 0, 0 };
    V a = Choose(f, true, true, t, z, 0, false, false, z);
    bool ok = Near(a.X, 0.6f) && Near(a.Y, 0.8f) && a.Z == 0.0f;
    V b = Choose(f, false, true, t, l, 1.5707963f, false, false, z);
    ok = ok && Near(b.X, 0.0f) && Near(b.Y, 1.0f);
    V c = Choose(f, false, false, t, l, 0.3f, true, true, t);
    ok = ok && Near(c.X, 0.0f) && Near(c.Y, 1.0f) && c.Z == 0.0f;
    V d = Choose(f, false, false, t, z, 0, false, true, t);
    ok = ok && Near(d.X, 0.6f) && Near(d.Y, 0.8f);
    V e = Choose(f, false, false, t, z, 0, false, false, t);
    ok = ok && e.X == f.X && e.Y == f.Y && e.Z == f.Z;
    std::printf(ok ? "006aabe0_TEST PASS\n" : "006aabe0_TEST FAIL\n");
    return ok ? 0 : 1;
}