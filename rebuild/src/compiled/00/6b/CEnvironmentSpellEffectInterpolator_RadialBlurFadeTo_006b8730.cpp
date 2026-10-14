// CEnvironmentSpellEffectInterpolator::RadialBlurFadeTo(long a, long b, float strength, long c, float radius, long d, long e, long f) -- forward to the vector overload with the two floats splat into 12-byte temporaries
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B8730, length 89 bytes.
// Callees: the vector overload @ 0x006B8550 (on `this`, ecx passed straight through; the manifest label is a propagated _Ucopy symbol). Shape: a 0x18-byte frame
// holding the two temporaries (the radius splat at the bottom, the strength splat above it) built as dword copies of the argument slots, eight pushes
// last-first; `add esp,18h ; ret 20h`.
struct C3DVector_006b8730 {
    float X, Y, Z;
    C3DVector_006b8730(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

struct CEnvironmentSpellEffectInterpolator {
    void RadialBlurFadeTo(long a, long b, const C3DVector_006b8730& strength, long c, const C3DVector_006b8730& radius, long d, long e, long f);   // 0x006B8550
    void RadialBlurFadeTo(long a, long b, float strength, long c, float radius, long d, long e, long f);
};

void CEnvironmentSpellEffectInterpolator::RadialBlurFadeTo(long a, long b, float strength, long c, float radius, long d, long e, long f)
{
    RadialBlurFadeTo(a, b, C3DVector_006b8730(strength, strength, strength), c, C3DVector_006b8730(radius, radius, radius), d, e, f);
}