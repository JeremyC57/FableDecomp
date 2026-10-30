// Behaviour test for 0x008520F0 NCameraModes::CScriptModuleFOVZoom::CalculateIncrementVector (see src/008520f0.cpp):
// Increment = (to - from) * (1 / frames), so `frames` steps of Increment carry the camera from `from` to `to`.
#include <cstdio>
#include <cmath>
struct V { float X, Y, Z; };
static V Calc(const V& from, const V& to, long frames)
{
    float s = 1.0f / frames;
    V r = { (to.X - from.X) * s, (to.Y - from.Y) * s, (to.Z - from.Z) * s };
    return r;
}
int main()
{
    V a = { 1.0f, 2.0f, 3.0f }, b = { 5.0f, -2.0f, 11.0f };
    V inc = Calc(a, b, 4);
    bool ok = std::fabs(inc.X - 1.0f) < 1e-6f && std::fabs(inc.Y + 1.0f) < 1e-6f && std::fabs(inc.Z - 2.0f) < 1e-6f;
    V p = a;
    for (int i = 0; i < 4; ++i) { p.X += inc.X; p.Y += inc.Y; p.Z += inc.Z; }
    ok = ok && std::fabs(p.X - b.X) < 1e-5f && std::fabs(p.Y - b.Y) < 1e-5f && std::fabs(p.Z - b.Z) < 1e-5f;
    std::printf(ok ? "008520f0_TEST PASS\n" : "008520f0_TEST FAIL\n");
    return ok ? 0 : 1;
}