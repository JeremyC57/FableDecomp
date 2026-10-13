#include <cstdio>
static int g_c = 0; struct O { void M() { ++g_c; } }; struct X { virtual void v00() {} virtual void F(O* o) { o->M(); } };
int main()
{
    O o; X x; x.F(&o); if (g_c != 1) { std::printf("00c36230_TEST FAIL\n"); return 1; }

    std::printf("00c36230_TEST PASS\n");
    return 0;
}
