// Behaviour test for 0x0057EF86 GFLimitReturn<float>(value, lo, hi): returns a reference to lo, hi or value.
#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#endif

template <class T> const T& __fastcall GFLimitReturn(const T& value, const T& lo, const T& hi)
{
    return value < lo ? lo : value > hi ? hi : value;
}

int main()
{
    float lo = -1.5f, hi = 2.25f;
    float below = -3.0f, above = 9.0f, inside = 0.5f, atLo = -1.5f, atHi = 2.25f;
    int ok = 1;
    if (&GFLimitReturn(below, lo, hi) != &lo) ok = 0;
    if (&GFLimitReturn(above, lo, hi) != &hi) ok = 0;
    if (&GFLimitReturn(inside, lo, hi) != &inside) ok = 0;
    if (&GFLimitReturn(atLo, lo, hi) != &atLo) ok = 0;            // equal to a bound: the value itself
    if (&GFLimitReturn(atHi, lo, hi) != &atHi) ok = 0;
    float v = GFLimitReturn(above, lo, hi);
    if (v < 2.2499f || v > 2.2501f) ok = 0;
    printf(ok ? "0057ef86_TEST PASS\n" : "0057ef86_TEST FAIL\n");
    return ok ? 0 : 1;
}