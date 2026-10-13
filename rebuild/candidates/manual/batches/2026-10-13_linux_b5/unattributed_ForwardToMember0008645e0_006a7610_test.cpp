#include <cstdio>
struct S; static void* g_s = 0; static void* g_a = 0; static long g_f = 1;
static long __fastcall Shim(S* s, void* a, long f) { g_s = s; g_a = a; g_f = f; return 5; }
struct F { S* t; long G(void* a) { return Shim(t, a, 0); } };
int main()
{
    F f; f.t = reinterpret_cast<S*>(&f); int x; if (f.G(&x) != 5 || g_s != &f || g_a != &x || g_f) { std::printf("006a7610_TEST FAIL\n"); return 1; }

    std::printf("006a7610_TEST PASS\n");
    return 0;
}
