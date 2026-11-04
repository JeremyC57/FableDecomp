// Behaviour test for 0x00746EC0 CTCStealth::GetPositionToMoveToForStabOn(CThing&, float&) const: a model; checks the position one unit back from the stab position along the hero-to-victim direction and the facing angle in turns wrapped into [0, 1).
#include <cstdio>
#include <cmath>
#include <vector>
#include <string>
#include <set>
#include <map>

static void Get(float sx, float sy, float sz, float hx, float hy, float hz, float* out, float& angle)
{
    float dx = sx - hx, dy = sy - hy, dz = sz - hz; float r = 1.0f / std::sqrt(dx * dx + dy * dy + dz * dz);
    dx *= r; dy *= r; dz *= r; out[0] = sx - dx; out[1] = sy - dy; out[2] = sz - dz;
    float a = (float)std::atan2(dx, dy) * 0.15915494f;
    if (a < 0.0f || a >= 1.0f) { a = (float)std::fmod((double)a, 1.0); if (a < 0.0f) a += 1.0f; }
    angle = a;
}
int main()
{
    int ok = 1; float p[3], a;
    Get(10, 0, 0, 0, 0, 0, p, a); ok &= std::fabs(p[0] - 9) < 1e-5f && std::fabs(a - 0.25f) < 1e-5f;
    Get(-10, 0, 0, 0, 0, 0, p, a); ok &= std::fabs(p[0] + 9) < 1e-5f && std::fabs(a - 0.75f) < 1e-5f;
    std::printf(ok ? "00746EC0_TEST PASS\n" : "00746EC0_TEST FAIL\n"); return ok ? 0 : 1;
}
