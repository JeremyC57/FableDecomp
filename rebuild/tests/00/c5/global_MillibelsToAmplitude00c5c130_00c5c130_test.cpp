#include <cstdio>
#include <cmath>
// Behaviour model: silence at or below -10000 mB, else 10^(mB / 2000).
static float amp(long mb) { if (mb <= -10000) return 0.0f; return (float)std::pow(10.0, mb / 2000.0); }
int main()
{
    if (amp(-10000) != 0.0f || std::fabs(amp(0) - 1.0f) > 1e-6f || std::fabs(amp(-2000) - 0.1f) > 1e-6f) { std::printf("00c5c130_TEST FAIL\n"); return 1; }
    std::printf("00c5c130_TEST PASS\n");
    return 0;
}