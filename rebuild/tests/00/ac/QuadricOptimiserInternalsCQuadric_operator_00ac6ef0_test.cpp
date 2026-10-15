// Behaviour test for QuadricOptimiserInternals::CQuadric::operator*=(double) @ 0x00AC6EF0
#include <cstdio>

struct CQuadric {
    double AA, AB, AC, AD, BB, BC, BD, CC, CD, DD, Weight;
    void operator*=(double scale)
    {
        AA *= scale; AB *= scale; AC *= scale; AD *= scale;
        BB *= scale; BC *= scale; BD *= scale;
        CC *= scale; CD *= scale;
        DD *= scale;
        Weight *= scale;
    }
};

int main()
{
    CQuadric q;
    double* v = &q.AA;
    int i;
    for (i = 0; i < 11; ++i) v[i] = i + 1;
    q *= 2.5;
    for (i = 0; i < 11; ++i) {
        if (v[i] != (i + 1) * 2.5) { std::printf("00ac6ef0_TEST FAIL\n"); return 1; }
    }
    std::printf("00ac6ef0_TEST PASS\n");
    return 0;
}