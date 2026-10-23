#include <cstdio>
struct Fc; static Fc* g_o = 0; static void* g_v[4];
struct Ob { unsigned char b[0x2c]; Ob(Fc* o, void* b, void* c, void* d, void* a) { g_o = o; g_v[0] = a; g_v[1] = b; g_v[2] = c; g_v[3] = d; } };
struct Fc { Ob* Create(void* a, void* b, void* c, void* d) { return new Ob(this, b, c, d, a); } };
static bool Check() { Fc f; int a, b, c, d; Ob* o = f.Create(&a, &b, &c, &d); bool ok = o && g_o == &f && g_v[0] == &a && g_v[1] == &b && g_v[2] == &c && g_v[3] == &d; delete o; return ok; }

int main()
{
    if (!Check()) { std::printf("00c0e7e0_TEST FAIL\n"); return 1; }
    std::printf("0121101c_TEST PASS\n");
    return 0;
}