#include <cstdio>
#include <cstring>
// Behaviour of 0x00C40E40 (chunk 901 piece): log10(1000) is 3
#include <cmath>
namespace retail { float log10f_(float x) { return (float)::log10((double)x); } }
int main()
{
    float r = retail::log10f_(1000.0f);
    if (r < 2.999f || r > 3.001f) { std::printf("00c40e40_TEST FAIL\n"); return 1; }
    std::printf("00c40e40_TEST PASS\n"); return 0;
}