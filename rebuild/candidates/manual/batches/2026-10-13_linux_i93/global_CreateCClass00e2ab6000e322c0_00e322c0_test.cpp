#include <cstdio>
static int g_c = 0; struct X { unsigned char b[8]; X(void*, void*) { ++g_c; } };
static X* __fastcall C(void* a, void* b) { return new X(a, b); }
int main()
{
    X* x = C(0, 0); if (!x || g_c != 1) { std::printf("00e322c0_TEST FAIL\n"); return 1; }
    delete x;

    std::printf("00e322c0_TEST PASS\n");
    return 0;
}
