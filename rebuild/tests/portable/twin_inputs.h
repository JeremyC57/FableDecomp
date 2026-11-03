// twin_inputs.h -- the float inputs every float-to-long dual-path test probes, as IEEE-754 bit patterns (exact on every
// compiler): signed zeros, halves and near-halves (ties to even both ways), tiny and denormal magnitudes, the 2^23 / 2^24
// precision edges, the 32-bit limits (2147483520 is the largest float below 2^31; 2^31 and beyond store the integer
// indefinite 0x80000000), large magnitudes, infinities and a NaN. Used by rebuild/tests/portable/test_*.cpp.
#ifndef FABLE_TWIN_INPUTS_H
#define FABLE_TWIN_INPUTS_H
#include <string.h>

static const unsigned int kTwinInputBits[] = {
    0x00000000u, 0x80000000u,                           // +0, -0
    0x3F000000u, 0xBF000000u, 0x3FC00000u, 0xBFC00000u, // 0.5, -0.5, 1.5, -1.5
    0x40200000u, 0xC0200000u, 0x40600000u, 0xC0600000u, // 2.5, -2.5, 3.5, -3.5
    0x3EFFFFFFu, 0xBEFFFFFFu, 0x3F000001u, 0xBF000001u, // 0.49999997, -0.49999997, 0.50000006, -0.50000006
    0x3F7FFFFFu, 0xBF7FFFFFu, 0x3F800000u, 0xBF800000u, // 0.99999994, -0.99999994, 1, -1
    0x40133333u, 0xC0133333u, 0x402CCCCDu, 0xC02CCCCDu, // 2.3, -2.3, 2.7, -2.7
    0x3F400000u, 0xBF400000u, 0x42F6E979u, 0xC2F6E979u, // 0.75, -0.75, 123.456, -123.456
    0x2EDBE6FFu, 0xAEDBE6FFu, 0x30800000u, 0xB0800000u, // 1e-10, -1e-10, 2^-30, -2^-30
    0x33000000u, 0xB3000000u, 0x32800000u, 0xB2800000u, // 2^-25, -2^-25, 2^-26, -2^-26
    0x00000001u, 0x80000001u, 0x00800000u, 0x80800000u, // denormal min, -denormal min, FLT_MIN, -FLT_MIN
    0x4A7FFFFEu, 0xCA7FFFFEu, 0x4AFFFFFFu, 0xCAFFFFFFu, // 4194303.5, -4194303.5, 8388607.5, -8388607.5
    0x4B000000u, 0xCB000000u, 0x4B000001u, 0xCB000001u, // 2^23, -2^23, 2^23+1, -(2^23+1)
    0x4B800000u, 0xCB800000u, 0x4B800001u, 0xCB800001u, // 2^24, -2^24, 2^24+2, -(2^24+2)
    0x4EFFFFFFu, 0xCEFFFFFFu, 0x4F000000u, 0xCF000000u, // 2147483520, -2147483520, 2^31, -2^31
    0xCF000001u, 0x4F32D05Eu, 0xCF32D05Eu,              // -2147483904, 3e9, -3e9
    0x7149F2CAu, 0xF149F2CAu,                           // 1e30, -1e30
    0x7F800000u, 0xFF800000u, 0x7FC00000u, 0xFFC00000u  // +inf, -inf, NaN, -NaN
};
static const int kTwinInputCount = (int)(sizeof kTwinInputBits / sizeof kTwinInputBits[0]);

static float TwinInput(int i)
{
    float f;
    memcpy(&f, &kTwinInputBits[i], sizeof f);
    return f;
}
#endif
