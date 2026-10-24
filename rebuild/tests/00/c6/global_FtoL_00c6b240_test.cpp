// Behaviour test for 0x00C6B240 FtoL(float): fld + fistp dword, current FPU rounding mode (default round-to-nearest-even).
#include <cstdio>
#ifdef _MSC_VER
static long __cdecl FtoL(float f) 
{
    long result;
    __asm fld f
    __asm fistp result
    return result;
}
#else
static long FtoL(float f) { long result; int r; __asm__("fistpl %0" : "=m"(r) : "t"(f) : "st"); result = r; return result; }
#endif
int main()
{
    if (FtoL(1.4f) != 1 || FtoL(1.6f) != 2 || FtoL(2.5f) != 2 || FtoL(3.5f) != 4 || FtoL(-1.6f) != -2
        || FtoL(-2.5f) != -2 || FtoL(123456.0f) != 123456 || FtoL(0.0f) != 0) { std::printf("00c6b240_TEST FAIL\n"); return 1; }
    std::printf("00c6b240_TEST PASS\n");
    return 0;
}
