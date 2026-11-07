// Behaviour test for 0x006A9690 CThingPlayerCreature::ResolveRelativeMovementAcceleration: a g++ model of the parked source (the residue is codegen only).
#include <cstdio>
#include <cmath>
struct V { float x, y, z; };
static const float kEps = 1e-5f;
static void Normalise(V& v) { float l = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z); if (l > kEps) { v.x /= l; v.y /= l; v.z /= l; } }
static void RotateZ(V& v, float a) { float c = std::cos(a), s = std::sin(a); float x = v.x * c - v.y * s; float y = v.x * s + v.y * c; v.x = x; v.y = y; }
struct Player { unsigned char flags; V relAccel, relDir, velocity, pendingFacing; float pendingTurn; bool blocking; float maxSpeed; };
static void Resolve(Player& p, float controlAngle)
{
    p.flags &= ~1;
    V a = p.relAccel; Normalise(a); p.relDir = a;
    RotateZ(a, controlAngle);
    a.x *= p.maxSpeed; a.y *= p.maxSpeed; a.z *= p.maxSpeed;
    if (p.blocking) {
        if (std::fabs(p.pendingTurn) > kEps) { p.pendingTurn = 0.0f; p.pendingFacing.x = 1.0f; }
        p.velocity.x = p.velocity.y = p.velocity.z = 0.0f;
    }
    if (a.x * a.x + a.y * a.y + a.z * a.z > kEps * kEps) { p.velocity.x += a.x; p.velocity.y += a.y; p.velocity.z += a.z; p.flags |= 1; }
    p.relAccel.x = p.relAccel.y = p.relAccel.z = 0.0f;
}
int main() {
    Player p = { 0xff, { 0.0f, 2.0f, 0.0f }, { 0, 0, 0 }, { 1.0f, 1.0f, 1.0f }, { 0, 0, 0 }, 0.5f, true, 3.0f };
    Resolve(p, 1.5707963f);
    int ok = (p.flags & 1) && std::fabs(p.relDir.y - 1.0f) < 1e-6f && std::fabs(p.velocity.x + 3.0f) < 1e-5f && std::fabs(p.velocity.y) < 1e-5f && p.velocity.z == 0.0f
        && p.pendingTurn == 0.0f && p.pendingFacing.x == 1.0f && p.relAccel.y == 0.0f;
    Player q = { 0x01, { 0, 0, 0 }, { 0, 0, 0 }, { 1.0f, 0, 0 }, { 0, 0, 0 }, 0.0f, false, 3.0f };
    Resolve(q, 0.0f);
    ok = ok && !(q.flags & 1) && q.velocity.x == 1.0f;
    std::printf(ok ? "006a9690_TEST PASS\n" : "006a9690_TEST FAIL\n");
    return ok ? 0 : 1;
}