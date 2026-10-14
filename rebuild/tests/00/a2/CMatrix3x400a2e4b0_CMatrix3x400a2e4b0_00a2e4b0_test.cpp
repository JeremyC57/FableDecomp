#include <cstdio>
#include <new>
struct CMatrix3x4 { float m[4][3]; CMatrix3x4() { m[3][2] = 0.0f; m[3][1] = 0.0f; m[3][0] = 0.0f; m[2][1] = 0.0f; m[2][0] = 0.0f; m[1][2] = 0.0f; m[1][0] = 0.0f; m[0][2] = 0.0f; m[0][1] = 0.0f; m[2][2] = 1.0f; m[1][1] = 1.0f; m[0][0] = 1.0f; } };
int main()
{
    unsigned char raw[sizeof(CMatrix3x4)]; for (int k = 0; k < (int)sizeof(raw); ++k) raw[k] = 0xAA;
    CMatrix3x4* p = new (raw) CMatrix3x4;
    for (int r = 0; r < 4; ++r) for (int c = 0; c < 3; ++c) if (p->m[r][c] != ((r == c) ? 1.0f : 0.0f)) { std::printf("00a2e4b0_TEST FAIL\n"); return 1; }
    std::printf("00a2e4b0_TEST PASS\n"); return 0;
}