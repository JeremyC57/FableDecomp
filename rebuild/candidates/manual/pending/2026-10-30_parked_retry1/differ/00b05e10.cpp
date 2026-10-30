// NParticleEngine::CPSCLight::SmoothFadeValue(float t) -- cosine-table smooth fade: 0.5 - 0.5 * lerp(cos[i], cos[i+1]) over the 1024-entry table
// Grade: RELOCATION_MATCH bytes (retry1) -- HELD for the owner's triplicate-identity decision: same body and symbol as landed 0x00AF9000
// Leaf: SmoothFadeValue
// Flags: /O2 /Oy /W3
// Retail-asm: 0x00AF9021 `fld dword ptr [esp+4]` + 0x00AF9025 `fistp dword ptr [esp]` -- the engine's inline-assembly float-to-long helper: the argument (t - 0.5) is stored to the helper's by-value parameter copy and reloaded for a bare dword fistp into a frame word; VC7.1 C++ `(long)x` calls __ftol2 and /QIfist emits `fistp qword` and folds the 0.5*1024 constant, so the dword fistp is the original's `__asm`.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF9000, length 99 bytes (identical bodies at 0x00B05E10, 0x00B1AD90).
// Globals: cosine table g_CosTable @ 0x013CD550 (1025 floats, built at start-up); constants 0.5f @ 0x0122F59C,
//          1024.0f @ 0x01230010, 1.0f @ 0x0122DED8.
// Shape: t scaled in place in its argument slot (`fst [esp+0Ch]`); `fild` of the rounded index gives the fraction;
// the index is masked to 10 bits for the two table reads.
struct CPSCLight {
    float SmoothFadeValue(float t);
};
extern float g_CosTable[1025];          // 0x013CD550
// the engine's inline-assembly float-to-long helper: VC7.1 expands a __forceinline function that contains __asm in place
__forceinline long FloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}
float CPSCLight::SmoothFadeValue(float t)
{
    t = t * 0.5f;
    t = t * 1024.0f;
    long i = FloatToLong(t - 0.5f);
    float frac = t - (float)i;
    i &= 0x3ff;
    return 0.5f - ((1.0f - frac) * g_CosTable[i] + frac * g_CosTable[i + 1]) * 0.5f;
}