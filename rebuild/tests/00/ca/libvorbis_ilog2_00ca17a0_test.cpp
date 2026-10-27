#include <cstdio>
// Behaviour model: libvorbis ilog2 -- bits needed for v - 1.
static int ilog2(unsigned int v) { int ret = 0; if (v) --v; while (v) { ret++; v >>= 1; } return ret; }
int main()
{
    if (ilog2(0) != 0 || ilog2(1) != 0 || ilog2(2) != 1 || ilog2(256) != 8 || ilog2(257) != 9 || ilog2(2048) != 11) { std::printf("00ca17a0_TEST FAIL\n"); return 1; }
    std::printf("00ca17a0_TEST PASS\n");
    return 0;
}