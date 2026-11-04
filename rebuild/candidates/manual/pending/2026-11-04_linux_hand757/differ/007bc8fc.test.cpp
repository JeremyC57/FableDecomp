// Behaviour test for 0x007BC8FC NThingShot::CTCShotDefinitionTearInGround::InitMovement: a g++ model of the parked source (the residue is codegen only).
#include <cstdio>
#include <cmath>
struct S { float px, py, pz, vx, vy; int tcs; int updates; };
static void Init(S& s, bool targetAlive, float tx, float ty, float ground, float speed, float fps)
{
    if (!targetAlive) return;
    float dx = tx - s.px, dy = ty - s.py, l = std::sqrt(dx * dx + dy * dy); dx /= l; dy /= l;
    s.px += dx * 2.0f; s.py += dy * 2.0f; s.pz = ground; s.vx = dx * (speed / fps); s.vy = dy * (speed / fps); s.tcs++; s.updates++;
}
int main() {
    S s = { 0, 0, 5, 0, 0, 0, 0 }; Init(s, true, 3.0f, 4.0f, 1.0f, 30.0f, 30.0f);
    int ok = std::fabs(s.px - 1.2f) < 1e-5f && std::fabs(s.py - 1.6f) < 1e-5f && s.pz == 1.0f && std::fabs(s.vx - 0.6f) < 1e-5f && s.tcs == 1;
    S t = { 0, 0, 5, 0, 0, 0, 0 }; Init(t, false, 1, 1, 0, 1, 1); ok &= t.tcs == 0 && t.pz == 5;
    std::printf(ok ? "007bc8fc_TEST PASS\n" : "007bc8fc_TEST FAIL\n");
    return ok ? 0 : 1;
}
