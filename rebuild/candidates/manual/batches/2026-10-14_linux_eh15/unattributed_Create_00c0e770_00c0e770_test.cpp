#include <cstdio>
struct Fc; static Fc* g_o = 0; static void* g_a = 0; static void* g_b = 0; static void* g_c = 0;
struct Ob { unsigned char b[0x28]; Ob(Fc* o, void* b, void* c, void* a) { g_o = o; g_a = a; g_b = b; g_c = c; } };
struct Fc { Ob* Create(void* a, void* b, void* c) { return new Ob(this, b, c, a); } };
static bool Check() { Fc f; int a, b, c; Ob* o = f.Create(&a, &b, &c); bool ok = o && g_o == &f && g_a == &a && g_b == &b && g_c == &c; delete o; return ok; }

int main()
{
    if (!Check()) { std::printf("00c0e770_TEST FAIL\n"); return 1; }
    std::printf("00c0e770_TEST PASS\n");
    return 0;
}
