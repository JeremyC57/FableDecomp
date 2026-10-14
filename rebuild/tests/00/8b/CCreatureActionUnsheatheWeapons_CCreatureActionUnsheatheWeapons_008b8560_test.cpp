#include <cstdio>
static long g_code = -1;
struct Base { virtual ~Base() {} unsigned char body[0x110]; Base(long c) { g_code = c; } };
struct Act : Base { long m_mode; Act() : Base(150), m_mode(2) {} };
int main()
{
    Act a;
    if (g_code != 150 || a.m_mode != 2) { std::printf("008b8560_TEST FAIL\n"); return 1; }
    std::printf("008b8560_TEST PASS\n");
    return 0;
}