// behaviour test for 0x00C40E60 CUnprovenPow10 (model: 10^x)
#include <cstdio>
#include <cmath>
int main()
{
    float a = (float)std::pow(10.0, 2.0), b = (float)std::pow(10.0, -1.0);
    int ok = std::fabs(a - 100.0f) < 1e-3f && std::fabs(b - 0.1f) < 1e-6f;
    if (!ok) { std::printf("00c40e60_TEST FAIL\n"); return 1; }
    std::printf("00c40e60_TEST PASS\n");
    return 0;
}
