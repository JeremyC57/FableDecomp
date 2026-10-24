// Behaviour test for 0x00C89BE0 FtoL(double) __stdcall: fld qword + fistp dword, current FPU rounding mode.
#include <cstdio>
#ifdef _MSC_VER
static long __stdcall FtoL(double d) 
{
    long result;
    __asm fld d
    __asm fistp result
    return result;
}
#else
static long FtoL(double d) { long result; int r; __asm__("fistpl %0" : "=m"(r) : "t"(d) : "st"); result = r; return result; }
#endif
int main()
{
    if (FtoL(1.4) != 1 || FtoL(1.6) != 2 || FtoL(2.5) != 2 || FtoL(3.5) != 4 || FtoL(-1.6) != -2
        || FtoL(-2.5) != -2 || FtoL(2000000000.4) != 2000000000L || FtoL(0.0) != 0) { std::printf("00c89be0_TEST FAIL\n"); return 1; }
    std::printf("00c89be0_TEST PASS\n");
    return 0;
}
