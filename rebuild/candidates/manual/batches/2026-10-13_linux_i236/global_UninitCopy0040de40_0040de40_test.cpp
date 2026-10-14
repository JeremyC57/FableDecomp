#include <cstdio>
#include <new>
struct E { long v[1]; }; struct A { };
static E* __fastcall F(const E* f, const E* l, E* d, A) { for (; f != l; ++f, ++d) new (d) E(*f); return d; }
int main()
{
    E src[2]; src[0].v[0] = 5; src[1].v[0] = 6; E dst[2]; E* r = F(src, src + 2, dst, A()); if (r != dst + 2 || dst[0].v[0] != 5 || dst[1].v[0] != 6) { std::printf("0040de40_TEST FAIL\n"); return 1; }

    std::printf("0040de40_TEST PASS\n");
    return 0;
}
