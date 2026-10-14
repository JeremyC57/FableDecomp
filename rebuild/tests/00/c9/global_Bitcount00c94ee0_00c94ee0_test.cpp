#include <cstdio>
static unsigned long __fastcall F(unsigned long v) { unsigned long n = 0; while (v) { v >>= 1; ++n; } return n; }
int main()
{
    if (F(0) != 0 || F(1) != 1 || F(0x80) != 8) { std::printf("00c94ee0_TEST FAIL\n"); return 1; }

    std::printf("00c94ee0_TEST PASS\n");
    return 0;
}