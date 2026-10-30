// Behaviour test for libvorbis bitreverse (00c962b0): full 32-bit reversal.
#include <cstdio>
typedef unsigned int ogg_uint32_t;
static ogg_uint32_t bitreverse(ogg_uint32_t x)
{
    x = ((x >> 16) & 0x0000ffff) | ((x << 16) & 0xffff0000);
    x = ((x >> 8) & 0x00ff00ff) | ((x << 8) & 0xff00ff00);
    x = ((x >> 4) & 0x0f0f0f0f) | ((x << 4) & 0xf0f0f0f0);
    x = ((x >> 2) & 0x33333333) | ((x << 2) & 0xcccccccc);
    return ((x >> 1) & 0x55555555) | ((x << 1) & 0xaaaaaaaa);
}
int main()
{
    if (bitreverse(1u) != 0x80000000u || bitreverse(0x80000000u) != 1u || bitreverse(0x0000ffffu) != 0xffff0000u
        || bitreverse(0x12345678u) != 0x1e6a2c48u || bitreverse(bitreverse(0xdeadbeefu)) != 0xdeadbeefu)
    { std::printf("00c962b0_TEST FAIL\n"); return 1; }
    std::printf("00c962b0_TEST PASS\n");
    return 0;
}