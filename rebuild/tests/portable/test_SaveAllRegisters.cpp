// test_SaveAllRegisters.cpp -- test of libpk/SaveAllRegisters.h: calls bracketed by the pushad / popad pair behave as
// unbracketed ones (both build paths; the portable twin is empty, so there is nothing else to compare).
#include <stdio.h>
#include "libpk/SaveAllRegisters.h"
#include "fable_test_mode.h"
static unsigned long Mix(unsigned long a, unsigned long b) { return a * 2654435761ul + (b ^ (a >> 3)); }
static unsigned long Bracketed(unsigned long a, unsigned long b)
{
    unsigned long r = 0, s = 0;
    LIBPK_SAVE_ALL_REGISTERS();
    r = Mix(a, b);
    s = Mix(b, a);
    LIBPK_RESTORE_ALL_REGISTERS();
    return r ^ s;
}
int main()
{
    int bad = 0;
    for (unsigned long i = 0; i < 64; ++i)
        if (Bracketed(i, i * 7 + 1) != (Mix(i, i * 7 + 1) ^ Mix(i * 7 + 1, i))) ++bad;
    printf("test_SaveAllRegisters %s: %s\n", kFableTestMode, bad ? "FAIL" : "PASS");
    return bad ? 1 : 0;
}
