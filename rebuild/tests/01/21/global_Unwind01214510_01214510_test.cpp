#include <cstdio>
static int g_c = 0, g_d = 0; static void* g_t = 0; static void* g_x = 0; struct Hp { unsigned char b[0x3c]; Hp(void* t, void* c) { g_t = t; g_x = c; } long C() { ++g_c; return 4; } ~Hp() { ++g_d; } }; struct X { unsigned char p[4]; void* c; long F(void* t) { Hp h(t, c); return h.C(); } };
static bool Check() { X x; int i, j; x.c = &j; return x.F(&i) == 4 && g_t == &i && g_x == &j && g_c == 1 && g_d == 1; }

int main()
{
    if (!Check()) { std::printf("00c36cf0_TEST FAIL\n"); return 1; }
    std::printf("00c36cf0_TEST PASS\n");
    return 0;
}