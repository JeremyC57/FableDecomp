#include <cstdio>
static bool g = false; static void __fastcall S(bool v) { g = v; }
int main()
{
    S(true); if (!g) { std::printf("009bc8a0_TEST FAIL\n"); return 1; }

    std::printf("009bc8a0_TEST PASS\n");
    return 0;
}