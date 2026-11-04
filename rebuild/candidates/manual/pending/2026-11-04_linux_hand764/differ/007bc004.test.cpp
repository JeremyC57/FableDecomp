// Behaviour test for 0x007BC004 NThingShot::CTCShotDefinitionNymphFun::InitMovement(): a model; checks nothing for a dead target, else the aim at the target ground point, the normalised direction, a level side axis perpendicular to it, and the push at speed / fps along it.
#include <cstdio>
#include <cmath>
#include <vector>
#include <string>
#include <set>
#include <map>

struct V { float X, Y, Z; };
static bool Init(bool alive, V target, float ground, V pos, float speed, long fps, V& dir, V& side, V& vel)
{
    if (!alive) return false;
    V aim = { target.X, target.Y, ground };
    dir.X = aim.X - pos.X; dir.Y = aim.Y - pos.Y; dir.Z = aim.Z - pos.Z;
    float r = 1.0f / std::sqrt(dir.X * dir.X + dir.Y * dir.Y + dir.Z * dir.Z); dir.X *= r; dir.Y *= r; dir.Z *= r;
    V a = { dir.Y, -dir.X, 0.0f };
    side.X = a.Y * dir.Z - a.Z * dir.Y; side.Y = a.Z * dir.X - a.X * dir.Z; side.Z = a.X * dir.Y - a.Y * dir.X;
    r = 1.0f / std::sqrt(side.X * side.X + side.Y * side.Y + side.Z * side.Z); side.X *= r; side.Y *= r; side.Z *= r;
    float s = speed / fps; vel.X = dir.X * s; vel.Y = dir.Y * s; vel.Z = dir.Z * s; return true;
}
int main()
{
    int ok = 1; V d, s, v; V t = { 10, 0, 5 }, p = { 0, 0, 0 };
    ok &= !Init(false, t, 0, p, 30, 30, d, s, v);
    ok &= Init(true, t, 0, p, 30, 30, d, s, v) && std::fabs(d.X - 1) < 1e-5f && std::fabs(v.X - 1) < 1e-5f;
    ok &= std::fabs(s.X * d.X + s.Y * d.Y + s.Z * d.Z) < 1e-5f;
    std::printf(ok ? "007BC004_TEST PASS\n" : "007BC004_TEST FAIL\n"); return ok ? 0 : 1;
}
