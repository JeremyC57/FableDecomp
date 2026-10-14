#include <cstdio>
static int g_n = 0; struct I { virtual void M() = 0; }; struct J : I { void M() { ++g_n; } };
static void __stdcall F(I* object) { object->M(); }
int main()
{
    J j; F(&j); if (g_n != 1) { std::printf("00c107c0_TEST FAIL\n"); return 1; }

    std::printf("00c107c0_TEST PASS\n");
    return 0;
}