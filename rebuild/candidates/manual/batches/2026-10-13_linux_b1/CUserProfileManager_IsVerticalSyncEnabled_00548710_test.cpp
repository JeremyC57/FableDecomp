#include <cstdio>

struct M { unsigned char p0[0x0C]; bool UseNew; unsigned char p1[0x50 - 0x0D]; bool Cur; unsigned char p2[0x10c - 0x50 - 1]; bool New;
    bool Get() { if (UseNew) return New; return Cur; } };
int main()
{
    M m; m.Cur = (bool)1; m.New = (bool)0; m.UseNew = false; if (m.Get() != (bool)1) { std::printf("00548710_TEST FAIL\n"); return 1; }
    m.UseNew = true; if (m.Get() != (bool)0) { std::printf("00548710_TEST FAIL\n"); return 1; }
    std::printf("00548710_TEST PASS\n");
    return 0;
}
