#include <cstdio>
static int g_d = 0; static void __fastcall Destroy(void*) { ++g_d; }
struct B { long r; void (__fastcall* f)(void*); void* o; };
struct P { void* o; B* c; P& operator=(const P& x) { B* cc = x.c; void* oo = x.o; if (c != cc) { if (c) { --c->r; if (c->r == 0) { c->f(c->o); delete c; } } o = oo; c = cc; if (cc) ++cc->r; } return *this; } };
struct Tg { Tg() {} };
static P* __fastcall M(P* f, P* l, P* d, Tg, Tg) { for (long n = l - f; 0 < n; --n, ++f, ++d) *d = *f; return d; }
int main()
{
    B* b = new B; b->r = 1; b->f = Destroy; b->o = 0; P src[2]; src[0].o = 0; src[0].c = b; src[1].o = 0; src[1].c = 0; P dst[2]; dst[0].o = 0; dst[0].c = 0; dst[1].o = 0; dst[1].c = 0;
    P* e = M(src, src + 2, dst, Tg(), Tg()); if (e != dst + 2 || dst[0].c != b || b->r != 2 || g_d != 0) { std::printf("00a5fda0_TEST FAIL\n"); return 1; }

    std::printf("00a5fda0_TEST PASS\n");
    return 0;
}