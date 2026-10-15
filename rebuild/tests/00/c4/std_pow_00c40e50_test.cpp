#include <cstdio>
#include <cstring>
// Behaviour of 0x00C40E50 (chunk 901 piece): pow(2, 10) is 1024
#include <cmath>
namespace retail { float powf_(float x, float y) { float result = (float)::pow((double)x, (double)y); return result; } }
int main()
{
    float r = retail::powf_(2.0f, 10.0f);
    if (r != 1024.0f) { std::printf("00c40e50_TEST FAIL\n"); return 1; }
    std::printf("00c40e50_TEST PASS\n"); return 0;
}