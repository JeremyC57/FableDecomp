#include <cstdio>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
struct X; static void* g_v[7]; struct Fy { void* Cr(X* x, void* a, void* f, void* b, void* c, void* d, void* e) { g_v[0] = x; g_v[1] = a; g_v[2] = f; g_v[3] = b; g_v[4] = c; g_v[5] = d; g_v[6] = e; return (void*)&g_v; } }; struct Ow { unsigned char p[8]; Fy* f; };
struct X { unsigned char p[4]; Ow* o; void* Cr(void* a, void* b, void* c, void* d, void* e, void* f) { L l(&g_g); return o->f->Cr(this, a, f, b, c, d, e); } };
static bool Check() { Fy f; Ow o; o.f = &f; X x; x.o = &o; int a, b, c, d, e, g; void* r = x.Cr(&a, &b, &c, &d, &e, &g); return r == (void*)&g_v && g_v[0] == &x && g_v[1] == &a && g_v[2] == &g && g_v[3] == &b && g_v[4] == &c && g_v[5] == &d && g_v[6] == &e && g_l == 0; }

int main()
{
    if (!Check()) { std::printf("00c2ca40_TEST FAIL\n"); return 1; }
    std::printf("00c2ca40_TEST PASS\n");
    return 0;
}
