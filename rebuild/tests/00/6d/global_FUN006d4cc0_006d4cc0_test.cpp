#include <cstdio>
// Behaviour test for 006d4cc0: bit 0 of the byte at +0x1C9 read through a one-bit bitfield.
struct Owner { unsigned char pad[0x1C9]; unsigned char flag : 1; unsigned char other : 7;
    unsigned char Get() { return flag; } };
int main()
{
    Owner o; o.flag = 0; o.other = 0x7F;
    int ok = (o.Get() == 0);
    o.flag = 1; ok = ok && (o.Get() == 1);
    o.other = 0; ok = ok && (o.Get() == 1);
    if (!ok) { std::printf("006d4cc0_TEST FAIL\n"); return 1; }
    std::printf("006d4cc0_TEST PASS\n");
    return 0;
}