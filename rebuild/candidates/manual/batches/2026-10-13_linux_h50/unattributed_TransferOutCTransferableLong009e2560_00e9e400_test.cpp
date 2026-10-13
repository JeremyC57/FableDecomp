#include <cstdio>
struct Ctx; static long g_v = 0; static void __fastcall Helper(Ctx*, long n, long* v) { g_v = *v + n; }
struct X { virtual void v00() {} long* p; virtual void T(Ctx* c, long n) { long v = *p; Helper(c, n, &v); } };
int main()
{
    long val = 40; X x; x.p = &val; x.T(0, 2); if (g_v != 42) { std::printf("00e9e400_TEST FAIL\n"); return 1; }

    std::printf("00e9e400_TEST PASS\n");
    return 0;
}
