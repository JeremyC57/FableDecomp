// NParticleEngine::CPSCUpdateNormal::GetCurrentFadeValue() -- fade multiplier for the component: 1 unless fading is enabled; otherwise the normalised age is eased in (half-cosine table) before the packed fade-in end and eased out after the packed fade-out start
// Leaf: GetCurrentFadeValue
// Naming wave 35c (caller respell): calls 0x00AEAA00 under its PDB name NParticleEngine::GetOneOverTickRate (?GetOneOverTickRate@NParticleEngine@@YIMXZ; no arguments, so the __fastcall call site is unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B0A750, 0x1C8 bytes (padded to 464), `ret`, float in st0.
// Callees: 0x00AEAA00 (the particle clock, float). Data: the 1025-entry cosine table at 0x013CD550, 1/1023 at 0x01266300,
//   0.5f 0x0122F59C, the table scale 0x01230010, 2^32 0x0122DCB4.
// Layout: local model: packed fade word +0x0C (bits 0-9 fade-in end, 10-19 fade-out start, bit 25 enabled), lifetime
//   float +0x18, frame count long +0x98.
// Retail-asm: 0x00B0A848 / 0x00B0A8CC (fistp at 0x00B0A850 / 0x00B0A8D4) `fstp [slot] ; fld [slot] ; fistp dword ptr [slot]` in the inline table-cosine (x2) -- the engine's inline-assembly float-to-long helper (the by-value copy is stored and reloaded for a bare dword fistp; VC7.1 C++ `(long)x` calls __ftol2 and /QIfist emits `fistp qword` without the reload).
// Helper (2026-11-03 asm twins): the retail inline-asm block is the shared engine/GFFloatToLongNear.h (two-path: the byte-matching asm, and a FABLE_PORTABLE C++ twin); this row carries no asm of its own.
#include "fable_compiler.h"
extern float GFastCosTable[1025];                  // 0x013CD550 ?GFastCosTable@@3PAMA
extern const float g_fadeScale;                     // 0x01266300
extern const float g_cosTableScale;                 // 0x01230010
namespace NParticleEngine { float __fastcall GetOneOverTickRate(); }   // 0x00AEAA00
// the engine's inline-assembly float-to-long helper, expanded in place (VC7.1 homes the by-value parameter in a frame
// word, often a dead argument slot, and reloads it for the bare dword fistp)
#include "engine/GFFloatToLongNear.h"
FABLE_FORCEINLINE float TableCos01(float x)
{
    float scaled = x * 0.5f * g_cosTableScale;
    long whole = GFFloatToLongNear(scaled - 0.5f);
    float frac = scaled - (float)whole;
    long index = whole & 0x3ff;
    return 0.5f - ((1.0f - frac) * GFastCosTable[index] + frac * GFastCosTable[index + 1]) * 0.5f;
}
namespace NParticleEngine {
class CPSCUpdateNormal { public:
    unsigned char _pad00[0x0c];
    unsigned long FadeBits;                         // +0x0c
    unsigned char _pad10[0x08];
    float         Lifetime;                         // +0x18
    unsigned char _pad1c[0x98 - 0x1c];
    long          Frame;                            // +0x98 (multiplied as an integer operand: fimul)
    float GetCurrentFadeValue();
};
float CPSCUpdateNormal::GetCurrentFadeValue()
{
    if (!(FadeBits & 0x2000000))
        return 1.0f;
    float age = 0.0f;
    if (Lifetime > 0.0f) {
        age = (NParticleEngine::GetOneOverTickRate() * Frame) / Lifetime;
        if (age < 0.0f)
            age = 0.0f;
        else if (age > 1.0f)
            age = 1.0f;
    }
    float fadeIn = (float)(FadeBits & 0x3ff) * g_fadeScale;
    float fadeOut = (float)((FadeBits >> 10) & 0x3ff) * g_fadeScale;
    float result = 1.0f;
    if (age < fadeIn)
        result = TableCos01(age / fadeIn);
    if (age > fadeOut)
        result *= TableCos01((1.0f - age) / (1.0f - fadeOut));
    return result;
}
}