#include <cstdio>
static void* g_m = 0; struct S { bool M(void* m) { g_m = m; return true; } }; struct O { unsigned char pad[0x70]; S* s; };
struct F { void* vt; O* o; unsigned char pad[8]; unsigned char m[4]; bool G() { return o->s->M(&m[0]); } };
int main()
{
    S s; O o; o.s = &s; F f; f.o = &o; if (!f.G() || g_m != &f.m[0]) { std::printf("007f53a0_TEST FAIL\n"); return 1; }

    std::printf("007f53a0_TEST PASS\n");
    return 0;
}