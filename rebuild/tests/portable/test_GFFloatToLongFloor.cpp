// test_GFFloatToLongFloor.cpp -- dual-path test of engine/GFFloatToLongFloor.h (fistp(x - 0.5) + the bit-equal step up).
// Golden values: the retail asm path, VC7.1 RTM 3077 /O2 under Wine with the CRT default control word 0x027F (precision
// 53 bits, RC = nearest), one per input of twin_inputs.h. Build modes (tools/decomp_pipeline/portable_twins_check.py):
//   VC7.1 /DFABLE_DUAL_PATH_TEST   -- the asm path AND fable_portable::GFFloatToLongFloor: both must equal the golden values;
//   VC7.1 or g++ /DFABLE_PORTABLE  -- the portable path only (the header maps GFFloatToLongFloor to the twin).
#include <stdio.h>
#include "engine/GFFloatToLongFloor.h"
#include "twin_inputs.h"

static const unsigned int kGolden[] = {
    0x00000000u, 0x00000000u, 0x00000000u, 0xFFFFFFFFu,
    0x00000001u, 0xFFFFFFFEu, 0x00000002u, 0xFFFFFFFDu,
    0x00000003u, 0xFFFFFFFCu, 0x00000000u, 0xFFFFFFFFu,
    0x00000000u, 0xFFFFFFFFu, 0x00000000u, 0xFFFFFFFFu,
    0x00000001u, 0xFFFFFFFFu, 0x00000002u, 0xFFFFFFFDu,
    0x00000002u, 0xFFFFFFFDu, 0x00000000u, 0xFFFFFFFFu,
    0x0000007Bu, 0xFFFFFF84u, 0x00000000u, 0xFFFFFFFFu,
    0x00000000u, 0xFFFFFFFFu, 0x00000000u, 0xFFFFFFFFu,
    0x00000000u, 0xFFFFFFFFu, 0x00000000u, 0x00000000u,
    0x00000000u, 0x00000000u, 0x003FFFFFu, 0xFFC00000u,
    0x007FFFFFu, 0xFF800000u, 0x00800000u, 0xFF800000u,
    0x00800001u, 0xFF7FFFFFu, 0x01000001u, 0xFF000000u,
    0x01000002u, 0xFEFFFFFEu, 0x7FFFFF81u, 0x80000081u,
    0x80000000u, 0x80000001u, 0x80000000u, 0x80000000u,
    0x80000000u, 0x80000000u, 0x80000000u, 0x80000000u,
    0x80000000u, 0x80000000u, 0x80000000u
};

int main()
{
    int bad = 0;
    if ((int)(sizeof kGolden / sizeof kGolden[0]) != kTwinInputCount) {
        printf("test_GFFloatToLongFloor FAIL (golden table size)\n");
        return 1;
    }
    for (int i = 0; i < kTwinInputCount; ++i) {
        float x = TwinInput(i);
        unsigned int got = (unsigned int)GFFloatToLongFloor(x);
        if (got != kGolden[i]) {
            printf("  input %08X: build path %08X, golden %08X\n", kTwinInputBits[i], got, kGolden[i]);
            ++bad;
        }
#if defined(FABLE_DUAL_PATH_TEST) && !defined(FABLE_PORTABLE)
        unsigned int twin = (unsigned int)fable_portable::GFFloatToLongFloor(x);
        if (twin != got) {
            printf("  input %08X: asm %08X, portable twin %08X\n", kTwinInputBits[i], got, twin);
            ++bad;
        }
#endif
    }
#if defined(FABLE_DUAL_PATH_TEST) && !defined(FABLE_PORTABLE)
    const char* mode = "asm+portable";
#elif defined(FABLE_PORTABLE)
    const char* mode = "portable";
#else
    const char* mode = "asm";
#endif
    printf("test_GFFloatToLongFloor %s: %s (%d inputs)\n", mode, bad ? "FAIL" : "PASS", kTwinInputCount);
    return bad ? 1 : 0;
}
