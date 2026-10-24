#include <cstdio>
extern "C" long __stdcall vorbis_ftoi(double f)
{
    long i;
    __asm fld f
    __asm fistp i
    return i;
}
int main()
{
    bool ok = vorbis_ftoi(2.25) == 2 && vorbis_ftoi(-7.75) == -8 && vorbis_ftoi(100.5) == 100 && vorbis_ftoi(101.5) == 102;   // round-to-nearest-even
    std::printf(ok ? "00ca3b10_TEST PASS\n" : "00ca3b10_TEST FAIL\n");
    return ok ? 0 : 1;
}
