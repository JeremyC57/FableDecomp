#include <cstdio>

#include <cstring>
struct B { unsigned long s[0x121]; long init; B() { std::memset(s, 0, sizeof(s)); Init(); } void Init() { init = 1; } };
int main()
{
    B b; if (b.s[0] || b.s[0x120] || b.init != 1) { std::printf("00be32a0_TEST FAIL\n"); return 1; }
    std::printf("00be32a0_TEST PASS\n");
    return 0;
}