#include <cstdio>

static long g_fill = -1;
struct S { long pos; virtual long GetPosition() { return pos; } void WriteFill(long, long n) { g_fill = n; }
    void AlignData(long a) { long p = GetPosition(); if (p & (a - 1)) WriteFill(0, ((p + a - 1) & ~(a - 1)) - p); } };
int main()
{
    S s; s.pos = 16; s.AlignData(16); if (g_fill != -1) { std::printf("009942d0_TEST FAIL\n"); return 1; }
    s.pos = 13; s.AlignData(16); if (g_fill != 3) { std::printf("009942d0_TEST FAIL\n"); return 1; }
    std::printf("009942d0_TEST PASS\n");
    return 0;
}
