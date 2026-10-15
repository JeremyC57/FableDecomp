// Behaviour test for 0x007002A0: AsymmetricPyramid::Match = 1 - max(scaled x distance, scaled y distance).
#include <cstdio>
struct M { long XA, YA; float XC, NX, PX, YC, NY, PY;
  float Match(const float* o) const { const float& x = o[XA]; float dx; if (x < XC) dx = (XC - x) * NX; else dx = (x - XC) * PX;
    const float& y = o[YA]; float dy; if (y < YC) dy = (YC - y) * NY; else dy = (y - YC) * PY; if (dx > dy) return 1.0f - dx; return 1.0f - dy; } };
int main()
{
    M m = {0, 1, 0.0f, 0.5f, 0.25f, 0.0f, 1.0f, 2.0f};
    float o1[2] = {-1.0f, 0.125f};   // dx = 0.5, dy = 0.25 -> 0.5
    float o2[2] = {2.0f, -0.75f};    // dx = 0.5, dy = 0.75 -> 0.25
    float o3[2] = {0.0f, 0.0f};      // 1
    bool ok = m.Match(o1) == 0.5f && m.Match(o2) == 0.25f && m.Match(o3) == 1.0f;
    if (!ok) { std::printf("007002a0_TEST FAIL\n"); return 1; }
    std::printf("007002a0_TEST PASS\n");
    return 0;
}
