// NThingShot::RandomiseShotDirection(const C3DVector& direction, long accuracy, unsigned long* seed) -- scatter a shot: unless a 0..99 roll is at or under the accuracy, tilt the direction by a small random angle about a random axis perpendicular to it
// Grade: DIFFER(274v288) -- retail builds the cross product in a temporary, storing X, Y, Z in turn (fstp) and copying each by `mov reg,[t] ; mov [axis],reg`; ours keeps X/Y on the x87 stack and forwards them (only Z goes through the temp). Tried: return C3DVector(...), named-local result, member stores, inline SetCrossProduct. Next: `(float)(double)(...)` around each component (the landed 0x007A1870 CrossProduct) to force the in-order rounding stores.
// Leaf: ?RandomiseShotDirection@NThingShot@@YI?AVC3DVector@@ABV2@JPAK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BEBE0, 288 bytes, `__fastcall` (hidden result pointer in ecx, direction in edx,
//   accuracy + seed on the stack, `ret 8`). PDB: ?RandomiseShotDirection@NThingShot@@YI?AVC3DVector@@ABV2@JPAK@Z.
// Callees: GFROR13 0x00497890 and GFFloatRandom 0x004978A0 (both __fastcall), C3DVector::Rotate 0x00A13C60,
//   C3DVector::Normalise 0x00A14440. Constants: 0.0 0x0122DEDC, 1/48 0x0126E19C, 1/144 0x0126E170.
// Shape: the inline GFRandom (0x24A1/0x24DF LCG + GFROR13, `% 100` as an unsigned div) is compared signed with the
//   accuracy; the perpendicular axis is direction x (Y, -X, 0) inlined (the -X held in st(1), the 0.0 products kept
//   because VC cannot fold x*0), built in the inline cross product's result temporary and copied into a named local;
//   that axis is spun about the direction by a random angle, normalised, and the direction rotated about it by
//   rand/48 + 1/144. The named result is copied out through the hidden pointer (no NRVO in VC7.1).
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    void Rotate(const C3DVector& axis, float angle);    // 0x00A13C60
    void Normalise();                                   // 0x00A14440
};
inline C3DVector CrossProduct(const C3DVector& a, const C3DVector& b)
{
    C3DVector result(b.Z * a.Y - a.Z * b.Y, b.X * a.Z - a.X * b.Z, b.Y * a.X - a.Y * b.X);
    return result;
}

unsigned long __fastcall GFROR13(unsigned long value);                  // 0x00497890
float __fastcall GFFloatRandom(float range, unsigned long* seed);       // 0x004978A0
__forceinline unsigned long GFRandom(unsigned long range, unsigned long* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return range ? *seed % range : 0;
}

namespace NThingShot {
C3DVector __fastcall RandomiseShotDirection(const C3DVector& direction, long accuracy, unsigned long* seed)
{
    C3DVector result(direction);
    if ((long)GFRandom(100, seed) > accuracy) {
        C3DVector axis = CrossProduct(result, C3DVector(result.Y, -result.X, 0.0f));
        float spin = GFFloatRandom(1.0f, seed);
        axis.Rotate(result, spin);
        axis.Normalise();
        float tilt = GFFloatRandom(1.0f, seed) * (1.0f / 48.0f) + 1.0f / 144.0f;
        result.Rotate(axis, tilt);
    }
    return result;
}
}
