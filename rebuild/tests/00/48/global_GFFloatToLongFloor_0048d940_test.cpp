#include <cstdio>
#ifdef _MSC_VER
#pragma optimize("y", off)
 long GFFloatToLongFloor(float value)
{
    long result; float one; float half; float next;
    half = 0.5f; one = 1.0f;
    __asm {
        fld value
        fsub half
        fistp result
        fild result
        fadd one
        fstp next
        mov eax, value
        cmp eax, next
        jne done
        inc result
    done:
    }
    return result;
}
#else
#include <math.h>
long GFFloatToLongFloor(float value) { return (long)floor((double)value); }   // the FPU block is floor() for the magnitudes used here
#endif
int main()
{
    bool ok = GFFloatToLongFloor(2.0f) == 2 && GFFloatToLongFloor(2.7f) == 2 && GFFloatToLongFloor(3.0f) == 3
           && GFFloatToLongFloor(0.0f) == 0 && GFFloatToLongFloor(-0.5f) == -1 && GFFloatToLongFloor(-2.0f) == -2
           && GFFloatToLongFloor(2.5f) == 2 && GFFloatToLongFloor(1000.25f) == 1000;
    std::printf(ok ? "0048d940_TEST PASS\n" : "0048d940_TEST FAIL\n");
    return ok ? 0 : 1;
}