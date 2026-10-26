// CEnvironmentSpellEffectInterface::SetRadialBlurParams(float strength, long a, float radius, long b, long c, long d, long e) -- forward to the vector overload with the two floats splat into 12-byte temporaries
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008853A0, length 84 bytes.
// Callees: the vector overload @ 0x00885070 (on `this`, ecx passed straight through: the strength vector by address, a, the radius vector by address, b, c, d, e).
// Shape: a 0x18-byte frame holding the two temporaries (the radius splat at the bottom, the strength splat above it), built as dword copies of the argument
// slots, then the seven pushes last-first; `add esp,18h ; ret 1Ch`.
struct C3DVector_008853a0 {
    float X, Y, Z;
    C3DVector_008853a0(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

class CEnvironmentSpellEffectInterface { public:
    void SetRadialBlurParams(const C3DVector_008853a0& strength, long a, const C3DVector_008853a0& radius, long b, long c, long d, long e);   // 0x00885070
    void SetRadialBlurParams(float strength, long a, float radius, long b, long c, long d, long e);
};

void CEnvironmentSpellEffectInterface::SetRadialBlurParams(float strength, long a, float radius, long b, long c, long d, long e)
{
    SetRadialBlurParams(C3DVector_008853a0(strength, strength, strength), a, C3DVector_008853a0(radius, radius, radius), b, c, d, e);
}