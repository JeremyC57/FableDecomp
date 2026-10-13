#include <cstdio>
#include <new>
struct E { unsigned char b[16]; }; struct A {};
static E* __fastcall UC(E* f, E* l, E* dest, A) { E* d = dest; for (; f != l; ++f, ++d) new (d) E(*f); return d; }
int main()
{
    E src[2], dst[2]; src[0].b[0] = 1; src[1].b[0] = 3; E* e = UC(src, src + 2, dst, A()); if (e != dst + 2 || dst[1].b[0] != 3) { std::printf("00640220_TEST FAIL\n"); return 1; }

    std::printf("00640220_TEST PASS\n");
    return 0;
}