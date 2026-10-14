#include <cstdio>
static void* __cdecl R(void* b) { return b; } struct X { void* F(void* b) { return R(b); } };
int main()
{
    X x; int i; if (x.F(&i) != &i) { std::printf("00a3f990_TEST FAIL\n"); return 1; }

    std::printf("00a3f990_TEST PASS\n");
    return 0;
}