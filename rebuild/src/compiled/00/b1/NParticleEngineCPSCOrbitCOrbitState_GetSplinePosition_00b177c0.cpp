// NParticleEngine::CPSCOrbit::COrbitState::GetSplinePosition(float t) const -- the orbit's cubic Hermite position at t: FromPosition*(2t^3-3t^2+1) + ToPosition*(3t^2-2t^3) + FromTangent*(t^3-2t^2+t) + ToTangent*(t^3-t^2)
// Leaf: ?GetSplinePosition@COrbitState@CPSCOrbit@NParticleEngine@@QBE?AVC3DVector@@M@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B177C0, length 265 bytes, thiscall, hidden C3DVector result pointer
//   first (returned in eax), t second, `ret 8`. No direct callers (COrbitState::Update inlines it); it sits in the
//   CPSCOrbit TU (0x00B161A0 COrbitState::Initialise .. 0x00B17950 CPSCOrbit::DatabaseClone), right after the
//   0x00B17790 member that rotates FromPosition/ToPosition (+0x08/+0x14).
// Identity: FableWin.pdb `?GetSplinePosition@COrbitState@CPSCOrbit@NParticleEngine@@QBE?AVC3DVector@@M@Z` (a by-value
//   C3DVector from one float, const), and Ego_r's COrbitState layout puts exactly the four vectors this body reads at
//   its offsets: FromPosition +0x08, ToPosition +0x14, FromTangent +0x2C, ToTangent +0x38 (NextPosition +0x20 unread).
//   The body is the four cubic Hermite basis functions. The catalog label "Matrix_TransformVector" is wrong.
// Shape: t2 = t*t, t3 = t*t2; the 2t^3 term is `t3 + t3` shared by h1 and h2; h3 is homed in t's own argument slot.
//   The sum is the chained by-value C3DVector operators `P0*h1 + P1*h2 + T0*h3 + T1*h4`: VC7.1 evaluates each
//   operator+ argument before its left operand, so the products run h4, h3, h2, h1 and the adds fold back left to right.
//   The sum goes into a named `result` local that is then returned (VC7.1 has no NRVO): only that keeps all three
//   components on the x87 stack and stores them x, y, z at the end; `return <expression>;` stores x early (267v265).
//   Constants 3.0f @ 0x0122DED4, 1.0f @ 0x0122DED8.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }   // the engine's memberwise copy constructor
    const C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    const C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};

namespace NParticleEngine {

class CPSCOrbit {
public:
    class COrbitState {
    public:
        float     NextAngle;                   // +0x00
        float     RotateSpeedRandomValue;      // +0x04
        C3DVector FromPosition;                // +0x08
        C3DVector ToPosition;                  // +0x14
        C3DVector NextPosition;                // +0x20
        C3DVector FromTangent;                 // +0x2c
        C3DVector ToTangent;                   // +0x38
        C3DVector GetSplinePosition(float t) const;
    };
};

C3DVector CPSCOrbit::COrbitState::GetSplinePosition(float t) const
{
    float t2 = t * t;
    float t3 = t * t2;
    float h1 = 2.0f * t3 - 3.0f * t2 + 1.0f;
    float h2 = 3.0f * t2 - 2.0f * t3;
    float h3 = t3 - 2.0f * t2 + t;
    float h4 = t3 - t2;
    C3DVector result = FromPosition * h1 + ToPosition * h2 + FromTangent * h3 + ToTangent * h4;
    return result;
}

}   // namespace NParticleEngine