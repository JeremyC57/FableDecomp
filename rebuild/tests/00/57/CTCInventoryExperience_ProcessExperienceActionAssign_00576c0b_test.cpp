#include <cstdio>

static long g_stat = 0, g_ab = 0, g_up = 0;
struct X { void S(long i) { g_stat += i; } void A(long i) { g_ab += i; } void U() { ++g_up; }
    void Run() { long i; for (i = 0; i < 10; ++i) S(i); for (i = 1; i < 20; ++i) A(i); U(); } };
int main()
{
    X x; x.Run(); if (g_stat != 45 || g_ab != 190 || g_up != 1) { std::printf("00576c0b_TEST FAIL\n"); return 1; }
    std::printf("00576c0b_TEST PASS\n");
    return 0;
}