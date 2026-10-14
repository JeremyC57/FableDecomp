#include <cstdio>
static int g_n = 0; struct I { virtual void M() = 0; }; struct J : I { void M() { ++g_n; } };
static void __stdcall F(void* unused0, I* object) { object->M(); }
int main()
{
    J j; F(0, &j); if (g_n != 1) { std::printf("004cb100_TEST FAIL\n"); return 1; }

    std::printf("004cb100_TEST PASS\n");
    return 0;
}