#include <cstdio>
#include <new>
static int g_part = 0, g_pd = 0, g_bind = 0, g_data = 0; static void* g_binder = 0; struct V { float x, y, z; }; struct Pt { unsigned char b[0x180]; Pt() { ++g_part; } ~Pt() { ++g_pd; } void Bind(void* o) { ++g_bind; g_binder = o; } }; struct Vd { unsigned char b[0x1c]; Vd() { ++g_data; } ~Vd() {} };
struct X { V org; void* mc; void* m10; void* m14; Pt parts[2]; long m318; void* owner; Vd data; bool m33c; X(void* o, const V* v, void* a, void* b, void* c, void* bd) : org(*v), mc(a), m10(b), m14(c), m318(0), owner(o), data(), m33c(false) { for (long i = 0; i < 2; ++i) parts[i].Bind(bd); } };
static bool Check() { int o, a, b, c, bd; V v; v.x = 1; v.y = 2; v.z = 3; X* x = new X(&o, &v, &a, &b, &c, &bd); bool ok = x->org.z == 3 && x->mc == &a && x->m14 == &c && x->m318 == 0 && x->owner == &o && !x->m33c && g_part == 2 && g_bind == 2 && g_binder == &bd && g_data == 1; delete x; return ok && g_pd == 2; }

int main()
{
    if (!Check()) { std::printf("00c2d2f0_TEST FAIL\n"); return 1; }
    std::printf("00c2d2f0_TEST PASS\n");
    return 0;
}