// NParticleEngine::GFInterpolate(const C3DVector& a, const C3DVector& b, float t) -- linear interpolation of two vectors: a * (1 - t) + b * t, returned by value
// Leaf: ?GFInterpolate@NParticleEngine@@YI?AVC3DVector@@ABV2@0M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF3CF0, 99 bytes, __fastcall: hidden result pointer in ecx (returned in eax), a in edx,
//   b and t on the stack (`ret 8`).
// Identity: FableWin.pdb name set `?GFInterpolate@NParticleEngine@@YI?AVC3DVector@@ABV2@0M@Z` (the C3DVector overload of the
//   particle engine's GFInterpolate family, beside the GFInterpolateArray templates); no direct call sites (reached through
//   the particle parameter interpolation templates' COMDAT-folded or address-taken uses).
// Callees: none. Float 1.0 at 0x0122DED8.
// Label: catalog "InterpolateRadialBlurPositionParams" is descriptive.
// Shape: the expression `a * (1.0f - t) + b * t` over C3DVector's inline operator*(float) / operator+ by-value temporaries:
//   the right operand (b * t) is evaluated first.
#pragma pack(push, 1)
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
};
#pragma pack(pop)

namespace NParticleEngine {
C3DVector __fastcall GFInterpolate(const C3DVector& a, const C3DVector& b, float t)
{
    return a * (1.0f - t) + b * t;
}
}