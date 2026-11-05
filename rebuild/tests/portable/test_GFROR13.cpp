// test_GFROR13.cpp -- dual-path test of engine/GFROR13.h (GF_ROR13: `ror value, 13` on a variable in memory).
#include <stdio.h>
#include "engine/GFROR13.h"
#include "fable_test_mode.h"
static unsigned long Rotate(unsigned long v)
{
    GF_ROR13(v);
    return v;
}
int main()
{
    static const unsigned long in[] = {0ul, 1ul, 0x1FFFul, 0x2000ul, 0x80000000ul, 0xFFFFFFFFul, 0x12345678ul, 0xDEADBEEFul, 0x00002001ul};
    int bad = 0;
    for (int i = 0; i < (int)(sizeof in / sizeof in[0]); ++i) {
        unsigned int x = (unsigned int)in[i];
        unsigned long want = (unsigned long)((x >> 13) | (x << 19));
        unsigned long got = Rotate(in[i]);
        if ((got & 0xFFFFFFFFul) != want) { printf("  %08lX: %08lX, expected %08lX\n", in[i], got, want); ++bad; }
#ifdef FABLE_TEST_DUAL
        if (fable_portable::GFRotateRight13(in[i]) != got) { printf("  %08lX: twin differs\n", in[i]); ++bad; }
#endif
    }
    printf("test_GFROR13 %s: %s\n", kFableTestMode, bad ? "FAIL" : "PASS");
    return bad ? 1 : 0;
}
