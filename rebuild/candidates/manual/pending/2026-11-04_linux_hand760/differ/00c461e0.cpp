// CMatrix::InterpolateViewport(const C3DVector& factors, float time) -- trilinearly interpolate the eight corner values of a cube of 28-byte records by the three factors and aim the embedded interpolated float at the result over `time`
// Grade: DIFFER(116v163) -- dl=62 (try jr: one nested Lerp expression, const float& a/b): retail materialises x per edge pair in the dead factors slot (int copy, stored twice) and spills each twice-used lerp base to a 0x10 frame; Bilerp / pair-struct / LerpPair inline forms tried (LerpPair m gets the x slot copy once, 145v163)
// Leaf: InterpolateViewport
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C461E0, length 163 bytes (`ret 8`); library module (0x00C0xxxx..0x00C4xxxx), the catalog
// names are kept (class and method are descriptive, no PDB type).
// Callees: CInterpolatedFloat::SetTarget(float target, float rate, float time) @ 0x00C0B570 (landed 0x00C3A970 documents it), on the member
// at +0xE0.
// Layout: eight 0x1C-byte corner records (the value at +0x18 of each, index bit 2 = x, bit 1 = y, bit 0 = z), then the interpolated float.
// Shape: `a + (b - a) * t` lerps; the x factor is a named float parked in the dead `factors` argument slot; the result is stored straight
// into the reserved first-argument slot (`push ecx` then `fstp [esp]`).
struct C3DVector { float X, Y, Z; };
struct CCornerRecord {                          // 28 bytes
    unsigned char _pad[0x18];
    float         Value;                        // +0x18
};
struct CInterpolatedFloat {
    void SetTarget(float target, float rate, float time);   // 0x00C0B570
};
inline float Lerp(const float& a, const float& b, float t) { return a + (b - a) * t; }
struct CMatrix {
    CCornerRecord      Corners[8];              // +0x00
    CInterpolatedFloat Value;                   // +0xe0
    void InterpolateViewport(const C3DVector& factors, float time);
};

void CMatrix::InterpolateViewport(const C3DVector& factors, float time)
{
    Value.SetTarget(Lerp(Lerp(Lerp(Corners[0].Value, Corners[4].Value, factors.X), Lerp(Corners[2].Value, Corners[6].Value, factors.X), factors.Y),
                         Lerp(Lerp(Corners[1].Value, Corners[5].Value, factors.X), Lerp(Corners[3].Value, Corners[7].Value, factors.X), factors.Y), factors.Z), 0, time);
}
