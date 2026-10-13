#include <cstdio>
struct F { float X, Y; F(float x, float y) : X(x), Y(y) {} }; struct I { long X, Y; F To() { return F((float)X, (float)Y); } };
int main()
{
    I i; i.X = 3; i.Y = -4; F f = i.To(); if (f.X != 3.0f || f.Y != -4.0f) { std::printf("00c620b0_TEST FAIL\n"); return 1; }

    std::printf("00c620b0_TEST PASS\n");
    return 0;
}
