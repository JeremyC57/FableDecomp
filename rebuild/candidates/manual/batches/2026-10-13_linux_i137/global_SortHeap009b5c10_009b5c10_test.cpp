#include <cstdio>
struct P { long a; unsigned long b; P(const P& o) : a(o.a), b(o.b) {} P(long x) : a(x), b(0) {} }; struct L { char t; };
static int g_n = 0; static void __fastcall Adj(P* f, long h, long bot, P v, L) { g_n += (int)bot; f[h] = v; }
static void Pop(P* f, P* l, P* d, P v, L p) { *d = *f; Adj(f, 0, l - f, v, p); }
static void __fastcall SH(P* f, P* l, L p) { for (; 1 < l - f; --l) Pop(f, l - 1, l - 1, *(l - 1), p); }
int main()
{
    P v[3] = { P(3), P(1), P(2) }; L p; SH(v, v + 3, p); if (g_n != 3 || v[2].a != 3) { std::printf("009b5c10_TEST FAIL\n"); return 1; }

    std::printf("009b5c10_TEST PASS\n");
    return 0;
}
