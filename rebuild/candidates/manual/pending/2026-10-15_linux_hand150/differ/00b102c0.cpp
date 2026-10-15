// CPSCEmitterGeneric::GetRandomRadius(unsigned long* seed) const -- a random radius fraction biased by the emitter's packed radius exponent: rand01 ^ (2 ^ -bias), with the 10-bit packed bias decoded to [-5, 5] and rounded to two decimals
// Leaf: GetRandomRadius
// Grade: DIFFER(151v151) -- lane gap: retail rounds with a bare dword fistp after a float store/reload (inline-asm FloatToLong); our build also hoists the power-of-ten loop above the field decode
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B102C0, length 151 bytes (padded to 160).
// Callees: GFFloatRandom(float, unsigned long*) @ 0x004978A0 (__fastcall: seed in ecx, the float on the stack), _CIpow @ 0x00BFEB40 (x2).
// Globals: 4294967296.0f @ 0x0122DCB4, 1/1023 @ 0x0129EF84, 10.0f @ 0x01237F0C, 5.0f @ 0x012462A8, 1.0f @ 0x0122DED8, 2.0 @ 0x01244480.
// Shape: the same inline quantiser as CalcIsFinishedGeneratingParticles (unsigned-to-float, power-of-ten loop, inline-asm
// dword `fistp` rounding): lane gap.
float __fastcall GFFloatRandom(float range, unsigned long* seed);  // 0x004978A0 (seed in ecx, the float on the stack)
double __cdecl pow(double x, double y);
#pragma intrinsic(pow)

struct CPSCEmitterGeneric {
    unsigned char _pad00[0x38];
    unsigned long m_packedRadius;           // +0x38 (bits 10..19: radius bias)
    float GetRandomRadius(unsigned long* seed) const;
};

static __forceinline long FloatToLong(float f) { return (long)f; }
static __forceinline float RoundToDecimals(float value, long decimals)
{
    float scale = 1.0f;
    for (long i = decimals; i; --i)
        scale *= 10.0f;
    float scaled = value * scale;
    return FloatToLong(scaled) / scale;
}

float CPSCEmitterGeneric::GetRandomRadius(unsigned long* seed) const
{
    float r = GFFloatRandom(1.0f, seed);
    float bias = RoundToDecimals((float)((m_packedRadius >> 10) & 0x3ff) * (1.0f / 1023.0f) * 10.0f - 5.0f, 2);
    float exponent = (float)pow(2.0, -bias);
    return (float)pow(r, exponent);
}
