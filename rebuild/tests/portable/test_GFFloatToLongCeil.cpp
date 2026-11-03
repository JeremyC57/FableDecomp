// test_GFFloatToLongCeil.cpp -- dual-path test of engine/GFFloatToLongCeil.h (fistp(x + 0.5) - the bit-equal step back).
// Golden values: the retail asm path, VC7.1 RTM 3077 /O2 under Wine with the CRT default control word 0x027F (precision
// 53 bits, RC = nearest), one per input of twin_inputs.h. Build modes (tools/decomp_pipeline/portable_twins_check.py):
//   VC7.1 /DFABLE_DUAL_PATH_TEST   -- the asm path AND fable_portable::GFFloatToLongCeil: both must equal the golden values;
//   VC7.1 or g++ /DFABLE_PORTABLE  -- the portable path only (the header maps GFFloatToLongCeil to the twin).
#include <stdio.h>
#include "engine/GFFloatToLongCeil.h"
#include "twin_inputs.h"

static const unsigned int kGolden[] = {
    0x00000000u, 0x00000000u, 0x00000001u, 0x00000000u,
    0x00000002u, 0xFFFFFFFFu, 0x00000003u, 0xFFFFFFFEu,
    0x00000004u, 0xFFFFFFFDu, 0x00000001u, 0x00000000u,
    0x00000001u, 0x00000000u, 0x00000001u, 0x00000000u,
    0x00000001u, 0xFFFFFFFFu, 0x00000003u, 0xFFFFFFFEu,
    0x00000003u, 0xFFFFFFFEu, 0x00000001u, 0x00000000u,
    0x0000007Cu, 0xFFFFFF85u, 0x00000001u, 0x00000000u,
    0x00000001u, 0x00000000u, 0x00000001u, 0x00000000u,
    0x00000001u, 0x00000000u, 0x00000000u, 0x00000000u,
    0x00000000u, 0x00000000u, 0x00400000u, 0xFFC00001u,
    0x00800000u, 0xFF800001u, 0x00800000u, 0xFF800000u,
    0x00800001u, 0xFF7FFFFFu, 0x01000000u, 0xFEFFFFFFu,
    0x01000002u, 0xFEFFFFFEu, 0x7FFFFF7Fu, 0x8000007Fu,
    0x80000000u, 0x7FFFFFFFu, 0x80000000u, 0x80000000u,
    0x80000000u, 0x80000000u, 0x80000000u, 0x80000000u,
    0x80000000u, 0x80000000u, 0x80000000u
};

int main()
{
    int bad = 0;
    if ((int)(sizeof kGolden / sizeof kGolden[0]) != kTwinInputCount) {
        printf("test_GFFloatToLongCeil FAIL (golden table size)\n");
        return 1;
    }
    for (int i = 0; i < kTwinInputCount; ++i) {
        float x = TwinInput(i);
        unsigned int got = (unsigned int)GFFloatToLongCeil(x);
        if (got != kGolden[i]) {
            printf("  input %08X: build path %08X, golden %08X\n", kTwinInputBits[i], got, kGolden[i]);
            ++bad;
        }
#if defined(FABLE_DUAL_PATH_TEST) && !defined(FABLE_PORTABLE)
        unsigned int twin = (unsigned int)fable_portable::GFFloatToLongCeil(x);
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
    printf("test_GFFloatToLongCeil %s: %s (%d inputs)\n", mode, bad ? "FAIL" : "PASS", kTwinInputCount);
    return bad ? 1 : 0;
}
