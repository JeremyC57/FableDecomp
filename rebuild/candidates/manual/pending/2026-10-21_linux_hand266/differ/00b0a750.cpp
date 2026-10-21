// Grade: DIFFER(304v456) -- retail rounds with an inline `fld ; fistp` float-to-int helper (store/reload around fistp, no __ftol2): that is an inline-assembly idiom, not reachable by compiler-emitted C++ under byte purity; the cast form calls __ftol2 and the lane outlines the table lookup
// NParticleEngine::CPSCUpdateNormal::GetCurrentFadeValue() -- fade multiplier for the component: 1 unless fading is enabled; otherwise the normalised age is eased in (half-cosine table) before the packed fade-in end and eased out after the packed fade-out start
// Leaf: GetCurrentFadeValue
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B0A750, 0x1C8 bytes (padded to 464), `ret`, float in st0.
// Callees: 0x00AEAA00 (the particle clock, float). Data: the 1025-entry cosine table at 0x013CD550, 1/1023 at 0x01266300,
//   0.5f 0x0122F59C, the table scale 0x01230010, 2^32 0x0122DCB4.
// Layout: local model: packed fade word +0x0C (bits 0-9 fade-in end, 10-19 fade-out start, bit 25 enabled), lifetime
//   float +0x18, frame count long +0x98.
extern const float g_cosTable[1025];                // 0x013CD550
extern const float g_fadeScale;                     // 0x01266300
extern const float g_cosTableScale;                 // 0x01230010
float GetParticleClock();                           // 0x00AEAA00
inline long FloatToLong(float value) { return (long)value; }
inline float TableCos01(float x)
{
    float scaled = x * 0.5f * g_cosTableScale;
    long whole = FloatToLong(scaled - 0.5f);
    float frac = scaled - (float)whole;
    long index = whole & 0x3ff;
    return 0.5f - ((1.0f - frac) * g_cosTable[index] + frac * g_cosTable[index + 1]) * 0.5f;
}
namespace NParticleEngine {
struct CPSCUpdateNormal {
    unsigned char _pad00[0x0c];
    unsigned long FadeBits;                         // +0x0c
    unsigned char _pad10[0x08];
    float         Lifetime;                         // +0x18
    unsigned char _pad1c[0x98 - 0x1c];
    long          Frame;                            // +0x98
    float GetCurrentFadeValue();
};
float CPSCUpdateNormal::GetCurrentFadeValue()
{
    if (!(FadeBits & 0x2000000))
        return 1.0f;
    float age = 0.0f;
    if (Lifetime > 0.0f) {
        age = GetParticleClock() * Frame / Lifetime;
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
