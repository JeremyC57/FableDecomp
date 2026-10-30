// Behaviour test for 0x00C9EF70 todB (see src/00c9ef70.cpp): (bits(|x|) * 7.17711438e-7f - 764.6161886f), i.e. ~20*log10(|x|).
#include <cstdio>
#include <cstring>
#include <cmath>
static float todB(const float* x)
{
    int i; std::memcpy(&i, x, 4);
    i = i & 0x7fffffff;
    return (float)(i * 7.17711438e-7f - 764.6161886f);
}
int main()
{
    float one = 1.0f, neg = -10.0f, ten = 10.0f;
    float a = todB(&one), b = todB(&neg), c = todB(&ten);
    if (std::fabs(a) > 0.5f || b != c || std::fabs(c - 20.0f) > 0.6f) { std::printf("00c9ef70_TEST FAIL\n"); return 1; }
    std::printf("00c9ef70_TEST PASS\n"); return 0;
}