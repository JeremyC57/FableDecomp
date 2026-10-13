#include <cstdio>
static int g_n = 0; static const char* g_s = 0; extern "C" int __cdecl myputs(const char* s) { g_s = s; return 0; }
extern "C" void __cdecl myabort() { ++g_n; }
static void Raise() { myputs("basic_string"); myabort(); }
int main()
{
    Raise(); if (g_n != 1 || !g_s || g_s[0] != 'b') { std::printf("0099c550_TEST FAIL\n"); return 1; }

    std::printf("0099c550_TEST PASS\n");
    return 0;
}