#include <cstdio>
static long g_code = -1;
struct Base { virtual ~Base() {} unsigned char body[0x110]; Base(long c) { g_code = c; } };
struct Act : Base { unsigned char m_flag; Act() : Base(150), m_flag(0) {} };
int main()
{
    Act a;
    if (g_code != 150 || a.m_flag != 0) { std::printf("008b83c0_TEST FAIL\n"); return 1; }
    std::printf("008b83c0_TEST PASS\n");
    return 0;
}
