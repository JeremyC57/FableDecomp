#include <cstdio>
#include <new>
static int g_c = 0; struct X { unsigned char b[4]; X() {} X(const X& o) { ++g_c; b[0] = o.b[0]; } }; struct A {};
struct P { long f; X s; };
static P* __fastcall UC(P* f, P* l, P* dest, A) { P* d = dest; for (; f != l; ++f, ++d) new (d) P(*f); return d; }
int main()
{
    P src[2], dst[2]; src[1].f = 5; src[1].s.b[0] = 7; P* e = UC(src, src + 2, dst, A()); if (e != dst + 2 || g_c != 2 || dst[1].s.b[0] != 7) { std::printf("00824650_TEST FAIL\n"); return 1; }

    std::printf("00824650_TEST PASS\n");
    return 0;
}