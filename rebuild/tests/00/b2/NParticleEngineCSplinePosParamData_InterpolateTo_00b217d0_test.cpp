// Behaviour test for 0x00B217D0: flag switches past t = 0.5, array interpolated element-wise.
#include <cstdio>
struct P { bool keep; unsigned int v[2];
  void InterpolateTo(const P& o, float t) { keep = t > 0.5f ? o.keep : keep; for (int i = 0; i < 2; ++i) v[i] = (unsigned int)(v[i] + (o.v[i] - (float)v[i]) * t); } };
int main()
{
    P a = {false, {0, 10}}, b = {true, {10, 20}};
    P c = a; c.InterpolateTo(b, 0.5f); bool ok = !c.keep && c.v[0] == 5 && c.v[1] == 15;
    P d = a; d.InterpolateTo(b, 0.75f); ok = ok && d.keep;
    if (!ok) { std::printf("00b217d0_TEST FAIL\n"); return 1; }
    std::printf("00b217d0_TEST PASS\n");
    return 0;
}