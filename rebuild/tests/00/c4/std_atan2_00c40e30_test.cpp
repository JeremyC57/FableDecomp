#include <cstdio>
#include <cstring>
// Behaviour of 0x00C40E30 (chunk 901 piece): atan2(1, 1) is pi/4
#include <cmath>
namespace retail { float atan2f_(float y, float x) { return (float)::atan2((double)y, (double)x); } }
int main()
{
    float r = retail::atan2f_(1.0f, 1.0f);
    if (r < 0.785f || r > 0.786f) { std::printf("00c40e30_TEST FAIL\n"); return 1; }
    std::printf("00c40e30_TEST PASS\n"); return 0;
}