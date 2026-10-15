// CPSCLight::SmoothFadeValue(float t) -- raised-cosine fade: 0.5 - 0.5 * cos(pi * t) through the 1024-step cosine table with linear interpolation
// Grade: DIFFER(84v99) -- lane gap: retail rounds with a bare dword fistp after a float store/reload (inline-asm FloatToLong helper); /QIfist gives a qword fistp with no reload and folds the 0.5*1024 constant
// Flags tried: default, /QIfist
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF9000, length 99 bytes (identical bodies at 0x00B05E10, 0x00B1AD90).
// Globals: cosine table g_CosTable @ 0x013CD550 (1025 floats, built at start-up); constants 0.5f @ 0x0122F59C,
//          1024.0f @ 0x01230010, 1.0f @ 0x0122DED8.
// Shape: the table index is rounded by an inline float-to-int helper (`fstp [esp+4] ; fld [esp+4] ; fistp dword
// [esp]`: a by-value float parameter read back for a bare 32-bit fistp), which is an inline-asm helper in the
// original -- the lane cannot produce the bare dword fistp from C++.
struct CPSCLight {
    float SmoothFadeValue(float t);
};

extern float g_CosTable[1025];          // 0x013CD550

static __forceinline long FloatToLong(float f) { return (long)f; }

float CPSCLight::SmoothFadeValue(float t)
{
    t = t * 0.5f * 1024.0f;
    long i = FloatToLong(t - 0.5f);
    float frac = t - (float)i;
    i &= 0x3ff;
    return 0.5f - ((1.0f - frac) * g_CosTable[i] + frac * g_CosTable[i + 1]) * 0.5f;
}
