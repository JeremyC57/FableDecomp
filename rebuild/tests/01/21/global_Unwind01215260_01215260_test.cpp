#include <cstdio>
#include <cstdlib>
static int g_c = 0; struct X { unsigned char b[592]; static void* operator new(unsigned int n) { return std::malloc(n); } static void operator delete(void* p) { std::free(p); } X() { ++g_c; } };
static X* C() { return new X; }
int main()
{
    X* x = C(); if (!x || g_c != 1) { std::printf("00c3f970_TEST FAIL\n"); return 1; }
    delete x;

    std::printf("00c3f970_TEST PASS\n");
    return 0;
}