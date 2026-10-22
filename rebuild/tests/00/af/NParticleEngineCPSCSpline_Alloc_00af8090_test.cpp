#include <cstdio>
static int g_c = 0; struct X { unsigned char body[0x3c]; X() { ++g_c; } };
static X* Create() { return new X; }
int main()
{
    X* x = Create(); if (!x || g_c != 1) { std::printf("00af8090_TEST FAIL\n"); return 1; }
    delete x;

    std::printf("00af8090_TEST PASS\n");
    return 0;
}