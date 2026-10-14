#include <cstdio>

struct Mx { float m[4][4]; void Transpose() { Mx t = *this; for (int i = 0; i < 4; ++i) { m[i][0] = t.m[0][i]; m[i][1] = t.m[1][i]; m[i][2] = t.m[2][i]; m[i][3] = t.m[3][i]; } } };
int main()
{
    Mx a; for (int i = 0; i < 4; ++i) for (int j = 0; j < 4; ++j) a.m[i][j] = (float)(i * 10 + j);
    a.Transpose(); if (a.m[0][1] != 10.0f || a.m[3][0] != 3.0f || a.m[2][2] != 22.0f) { std::printf("00a5cab0_TEST FAIL\n"); return 1; }
    std::printf("00a5cab0_TEST PASS\n");
    return 0;
}