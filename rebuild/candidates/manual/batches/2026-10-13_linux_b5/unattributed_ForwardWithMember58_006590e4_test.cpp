#include <cstdio>
static void* g_a = 0; static void* g_m = 0; static void __stdcall H(void* a, void* m) { g_a = a; g_m = m; }
struct O { unsigned char pad[0x58]; void* M; void F(void* a) { H(a, &M); } };
int main()
{
    O o; int x; o.F(&x); if (g_a != &x || g_m != &o.M) { std::printf("006590e4_TEST FAIL\n"); return 1; }

    std::printf("006590e4_TEST PASS\n");
    return 0;
}
