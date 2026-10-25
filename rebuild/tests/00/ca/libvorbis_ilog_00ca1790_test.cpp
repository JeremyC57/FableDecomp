// Behaviour test for 00ca1790: libvorbis floor1.c ilog -- bit length of v
#include <cstdio>
extern "C" int __fastcall ilog(unsigned int v) { int ret = 0; while (v) { ret++; v >>= 1; } return (ret); }
int main()
{
    int ok = ilog(0) == 0 && ilog(1) == 1 && ilog(2) == 2 && ilog(3) == 2 && ilog(255) == 8 && ilog(256) == 9 && ilog(0x80000000u) == 32;
    std::printf(ok ? "00ca1790_TEST PASS\n" : "00ca1790_TEST FAIL\n");
    return ok ? 0 : 1;
}
