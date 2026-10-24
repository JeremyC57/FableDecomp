// CLoopedRange::GetFraction(float t) const -- interpolate between a start and an end fraction (wrapping through 1 when the end lies below the start) and clamp the result to [0, 1 - epsilon]
// Leaf: ?GetFraction@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077C360, 108 bytes (+ int3 padding), thiscall `ret 4`, result in st(0).
// Caller: NGraphicAppearance::CTCGraphicAppearanceSprite vtable slot 0 @ 0x0077C4D0 (`lea ecx,[esi-0Ch]`; the result
//   times an integer frame count picks the sprite frame).
// Constants: 1.0f @ 0x0122DED8, 0.0f @ 0x0122DEDC (literal pool), epsilon 0.0001f @ 0x0129BA3C (a global float: the
//   `1 - epsilon` bound is computed at run time and spilled to the dead `t` argument slot).
// Layout: the owning record's two fractions at +0x1C (End) and +0x20 (Start); no PDB symbol was found for this body, the
//   class and member names are descriptive.
// Shape: `if (End > Start) r = (End - Start) * t + Start; else { s = 1 - Start; r = s + (End - s) * t; }` then the
//   clamp `if (r < 0) return 0; max = 1 - epsilon; if (r > max) return max; return r;`.
extern const float g_epsilon_0129ba3c;                  // 0x0129BA3C (1e-4f)

struct CLoopedRange {
    unsigned char _pad_0x00[0x1C];
    float         End;                                  // +0x1C
    float         Start;                                // +0x20
    float GetFraction(float t) const;
};

float CLoopedRange::GetFraction(float t) const
{
    float r;
    if (End > Start)
        r = (End - Start) * t + Start;
    else {
        float s = 1.0f - Start;
        r = s + (End - s) * t;
    }
    if (r < 0.0f)
        return 0.0f;
    const float maxFraction = 1.0f - g_epsilon_0129ba3c;
    if (r > maxFraction)
        return maxFraction;
    return r;
}