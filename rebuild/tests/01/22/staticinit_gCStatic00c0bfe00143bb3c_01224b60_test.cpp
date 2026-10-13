#include <cstdio>
static int g_c = 0, g_d = 0; struct S { S(const char*, long) { ++g_c; } S() { ++g_c; } ~S() { ++g_d; } };
S g_one("x", -1); S g_two;
int main()
{
    if (g_c != 2 || g_d != 0) { std::printf("01224b60_TEST FAIL\n"); return 1; }

    std::printf("01224b60_TEST PASS\n");
    return 0;
}