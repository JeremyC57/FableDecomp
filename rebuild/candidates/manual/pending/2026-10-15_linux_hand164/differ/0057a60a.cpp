// CTCHeroStats::GetFatnessReducingFrequencyInFrames() const -- frames between fatness reductions: round(fps * def rate * 60) (ties rounded down), or fps * 2440 when the def rate is not positive
// Grade: DIFFER(71v116) lane gap -- retail rounds with a bare dword `fistp [ebp-4]` after storing/reloading the product and the 0.5f / 1.0f locals (an inline-asm round helper in the original: `fld v ; fadd half ; fistp r`, then `if (v == (float)r - 1.0f) --r;` compared as raw dwords); plain C++ calls __ftol2 instead (GOTCHAS: bare dword fistp lane gap, park after one try)
// Leaf: GetFatnessReducingFrequencyInFrames
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057A60A, length 116 bytes. Size-optimised TU with a frame pointer.
// Callees: CTCBase::GetDefinitions() @ 0x006869D0 (+0xDC: the hero-stats def; its fatness rate float at +0x80).
// Globals: the frames-per-second long 0x01375550, 0.0001f 0x0129BA3C, 60.0f 0x0124F2FC.
#pragma optimize("s", on)
extern long g_framesPerSecond_01375550;         // 0x01375550
extern const float g_minPositive_0129ba3c;      // 0x0129BA3C (0.0001f)
struct CHeroStatsDef {
    unsigned char _pad00[0x80];
    float FatnessReductionRate;                 // +0x80
};
struct CDefinitionHolder {
    unsigned char  _pad00[0xdc];
    CHeroStatsDef* PHeroStatsDef;               // +0xdc
};
class CThing;
class CTCHeroStats {
public:
    virtual ~CTCHeroStats();
    CThing* ParentThing;                        // +0x04
    CDefinitionHolder* GetDefinitions() const;  // 0x006869D0
    long GetFatnessReducingFrequencyInFrames() const;
};
inline long RoundToLong(float value)
{
    const float half = 0.5f;
    const float one = 1.0f;
    long result = (long)(value + half);
    float check = (float)result - one;
    if (*(long*)&value == *(long*)&check)
        --result;
    return result;
}

long CTCHeroStats::GetFatnessReducingFrequencyInFrames() const
{
    float rate = GetDefinitions()->PHeroStatsDef->FatnessReductionRate;
    if (rate > g_minPositive_0129ba3c)
        return RoundToLong((float)g_framesPerSecond_01375550 * rate * 60.0f);
    return g_framesPerSecond_01375550 * 2440;
}
