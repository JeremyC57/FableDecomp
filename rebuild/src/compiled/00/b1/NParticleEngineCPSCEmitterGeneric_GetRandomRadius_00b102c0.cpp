// NParticleEngine::CPSCEmitterGeneric::GetRandomRadius(unsigned long* seed) const -- a random radius: pow(random, 2^-bias) with the packed radius bias quantised to two decimals
// Leaf: GetRandomRadius
// Flags: /O2 /Oy /W3
// Retail-asm: 0x00B10328 `fld dword ptr [esp+10h]` + 0x00B1032C `fistp dword ptr [esp+4]` -- the engine's inline-assembly float-to-long helper (the scaled value stored to the helper's by-value parameter copy and reloaded for a bare dword fistp); VC7.1 C++ `(long)x` calls __ftol2 and /QIfist emits `fistp qword` without the reload, so the dword fistp is the original's `__asm`.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B102C0, length 151 bytes (padded to 160).
// Callees: GFFloatRandom(float, unsigned long*) @ 0x004978A0 (__fastcall: seed in ecx, the float on the stack), _CIpow @ 0x00BFEB40 (x2).
// Globals: 4294967296.0f @ 0x0122DCB4, 1/1023 @ 0x0129EF84, 10.0f @ 0x01237F0C, 5.0f @ 0x012462A8, 1.0f @ 0x0122DED8, 2.0 @ 0x01244480.
// Shape: the same inline quantiser as CalcIsFinishedGeneratingParticles (unsigned-to-float, power-of-ten loop, inline-asm
float __fastcall GFFloatRandom(float range, unsigned long* seed);  // 0x004978A0 (seed in ecx, the float on the stack)
extern "C" double __cdecl pow(double x, double y);
#pragma intrinsic(pow)

struct CPSCEmitterGeneric {
    unsigned char _pad00[0x38];
    unsigned long m_packedRadius;           // +0x38 (bits 10..19: radius bias)
    float GetRandomRadius(unsigned long* seed) const;
};

// the engine's inline-assembly float-to-long helper: VC7.1 expands a __forceinline function that contains __asm in place
static __forceinline long FloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}
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
    float unit = (float)((m_packedRadius >> 10) & 0x3ff) * (1.0f / 1023.0f);   // two statements: VC7.1 folds (1/1023)*10 otherwise
    float bias = RoundToDecimals(unit * 10.0f - 5.0f, 2);
    float exponent = (float)pow(2.0, -bias);
    return (float)pow(r, exponent);
}