#include <cstdio>
static void* g = 0; static void __fastcall S(void* v) { g = v; }
int main()
{
    int x; S(&x); if (g != &x) { std::printf("00aeaa80_TEST FAIL\n"); return 1; }

    std::printf("00aeaa80_TEST PASS\n");
    return 0;
}