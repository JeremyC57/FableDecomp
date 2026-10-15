// Behaviour test for 0x00F3A270: bytes needed to bit-pack all values at the widest value's bit width (dword-rounded).
#include <cstdio>
static long Calc(const unsigned int* v, unsigned int n)
{
    unsigned int mask = v[0]; unsigned int bits = 0;
    for (unsigned int i = 0; i < n; ++i) mask |= v[i];
    for (int bit = 31; bit >= 0; --bit) if (mask & (1u << bit)) { bits = bit + 1; break; }
    return (n * bits + 31) / 32 * 4;
}
int main()
{
    unsigned int a[3] = {1, 2, 5};          // width 3, 9 bits -> 4 bytes
    unsigned int b[40]; for (int i = 0; i < 40; ++i) b[i] = 0xff;   // 320 bits -> 40 bytes
    unsigned int c[2] = {0, 0};             // width 0 -> 0
    bool ok = Calc(a, 3) == 4 && Calc(b, 40) == 40 && Calc(c, 2) == 0;
    if (!ok) { std::printf("00f3a270_TEST FAIL\n"); return 1; }
    std::printf("00f3a270_TEST PASS\n");
    return 0;
}