#include <cstdio>

struct B { unsigned char pad[0x98]; unsigned long Alignment;
    unsigned long Align(unsigned long s) { unsigned long a = Alignment; return (s + a - 1) / a * a; } };
int main()
{
    B b; b.Alignment = 16;
    if (b.Align(17) != 32 || b.Align(32) != 32 || b.Align(1) != 16) { std::printf("009cbb90_TEST FAIL\n"); return 1; }
    std::printf("009cbb90_TEST PASS\n");
    return 0;
}
