// Behaviour test for 0x00C9FE80 icount (libvorbis res0.c): copy of the logic -- number of set bits.
#include <cstdio>
int icount(unsigned int v)
{
    int ret = 0;
    while (v) {
        ret += v & 1;
        v >>= 1;
    }
    return (ret);
}
int main()
{
    int ok = 1;
    ok &= icount(0) == 0;
    ok &= icount(1) == 1;
    ok &= icount(0xFF) == 8;
    ok &= icount(0x80000000u) == 1;
    ok &= icount(0xFFFFFFFFu) == 32;
    ok &= icount(0x12345678u) == 13;
    if (!ok) { std::printf("00c9fe80_TEST FAIL\n"); return 1; }
    std::printf("00c9fe80_TEST PASS\n");
    return 0;
}