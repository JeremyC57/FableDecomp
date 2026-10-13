#include <cstdio>
#include <new>
static int g_c = 0; struct E { unsigned char b[4]; E() {} E(const E& o) { ++g_c; b[0] = o.b[0]; } }; struct A {};
static E* __fastcall UC(E* f, E* l, E* dest, A) { E* d = dest; for (; f != l; ++f, ++d) new (d) E(*f); return d; }
int main()
{
    E src[3]; src[0].b[0] = 1; src[2].b[0] = 3; unsigned char buf[12]; E* e = UC(src, src + 3, (E*)buf, A()); if (e != (E*)(buf + 12) || g_c != 3 || ((E*)buf)[2].b[0] != 3) { std::printf("005bb1cf_TEST FAIL\n"); return 1; }

    std::printf("005bb1cf_TEST PASS\n");
    return 0;
}
