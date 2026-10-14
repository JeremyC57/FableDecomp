#include <cstdio>
static long g_code = 0; static int g_base = 0; static void* g_set = (void*)1;
struct Bip { virtual ~Bip() {} Bip() { ++g_base; } void SetPItem(void* p) { g_set = p; } };
struct Ip : Bip { Ip() : Bip() { SetPItem(0); } };
struct Base { virtual ~Base() {} unsigned char body[0x110]; Base(long c) { g_code = c; } };
struct Act : Base { Ip t; Act() : Base(250), t() {} };
int main()
{
    Act a;
    if (g_code != 250 || g_base != 1 || g_set != 0) { std::printf("008b9110_TEST FAIL\n"); return 1; }
    std::printf("008b9110_TEST PASS\n");
    return 0;
}
