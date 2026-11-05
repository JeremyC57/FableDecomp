// test_vorbis_ftoi.cpp -- dual-path test of libvorbis/vorbis_ftoi.h (VORBIS_FTOI: fld qword + fistp dword under RC =
// nearest-even). The inputs are twin_inputs.h's floats widened to double (exact), so the golden values are test_FtoL's
// (the same fistp of the same values). Build modes as test_FtoL.cpp.
#include <stdio.h>
#include "libvorbis/vorbis_ftoi.h"
#include "twin_inputs.h"
#include "fable_test_mode.h"

static const unsigned int kGolden[] = {
    0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
    0x00000002u, 0xFFFFFFFEu, 0x00000002u, 0xFFFFFFFEu,
    0x00000004u, 0xFFFFFFFCu, 0x00000000u, 0x00000000u,
    0x00000001u, 0xFFFFFFFFu, 0x00000001u, 0xFFFFFFFFu,
    0x00000001u, 0xFFFFFFFFu, 0x00000002u, 0xFFFFFFFEu,
    0x00000003u, 0xFFFFFFFDu, 0x00000001u, 0xFFFFFFFFu,
    0x0000007Bu, 0xFFFFFF85u, 0x00000000u, 0x00000000u,
    0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
    0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
    0x00000000u, 0x00000000u, 0x00400000u, 0xFFC00000u,
    0x00800000u, 0xFF800000u, 0x00800000u, 0xFF800000u,
    0x00800001u, 0xFF7FFFFFu, 0x01000000u, 0xFF000000u,
    0x01000002u, 0xFEFFFFFEu, 0x7FFFFF80u, 0x80000080u,
    0x80000000u, 0x80000000u, 0x80000000u, 0x80000000u,
    0x80000000u, 0x80000000u, 0x80000000u, 0x80000000u,
    0x80000000u, 0x80000000u, 0x80000000u
};

static long Ftoi(double f)
{
    long i;
    VORBIS_FTOI(i, f);
    return i;
}

int main()
{
    int bad = 0;
    if ((int)(sizeof kGolden / sizeof kGolden[0]) != kTwinInputCount) {
        printf("test_vorbis_ftoi FAIL (golden table size)\n");
        return 1;
    }
    for (int i = 0; i < kTwinInputCount; ++i) {
        double x = (double)TwinInput(i);
        unsigned int got = (unsigned int)Ftoi(x);
        if (got != kGolden[i]) {
            printf("  input %08X: build path %08X, golden %08X\n", kTwinInputBits[i], got, kGolden[i]);
            ++bad;
        }
#ifdef FABLE_TEST_DUAL
        if ((unsigned int)fable_portable::vorbis_ftoi(x) != got) {
            printf("  input %08X: twin differs\n", kTwinInputBits[i]);
            ++bad;
        }
#endif
    }
    printf("test_vorbis_ftoi %s: %s (%d inputs)\n", kFableTestMode, bad ? "FAIL" : "PASS", kTwinInputCount);
    return bad ? 1 : 0;
}
