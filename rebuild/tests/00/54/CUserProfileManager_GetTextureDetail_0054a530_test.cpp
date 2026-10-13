#include <cstdio>

struct M { unsigned char p0[0x0C]; bool UseNew; unsigned char p1[0x3c - 0x0D]; float Cur; unsigned char p2[0xf8 - 0x3c - 4]; float New;
    float Get() { if (UseNew) return New; return Cur; } };
int main()
{
    M m; m.Cur = (float)1; m.New = (float)0; m.UseNew = false; if (m.Get() != (float)1) { std::printf("0054a530_TEST FAIL\n"); return 1; }
    m.UseNew = true; if (m.Get() != (float)0) { std::printf("0054a530_TEST FAIL\n"); return 1; }
    std::printf("0054a530_TEST PASS\n");
    return 0;
}