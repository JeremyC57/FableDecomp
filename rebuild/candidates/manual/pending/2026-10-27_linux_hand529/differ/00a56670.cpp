// GFGetHermiteSplinePosition(C3DVector& result, const C3DVector& v1, const C3DVector& t1, const C3DVector& v2, const C3DVector& t2, float s) -- cubic Hermite interpolation: result = (2s^3-3s^2+1)*v1 + (3s^2-2s^3)*v2 + (s^3-2s^2+s)*t1 + (s^3-s^2)*t2
// Grade: DIFFER(295v295) -- only the final result copy is scheduled differently: retail loads [esp+14h] after the last fadd and stores [ecx+4] after the fstp; ours hoists both (user copy ctor, user operator=, QFE 4035 compiler all tried)
// Leaf: ?GFGetHermiteSplinePosition@@YIXAAVC3DVector@@ABV1@111M@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A56670, 295 bytes, `__fastcall` (result in ecx, v1 in edx, the other
//   three references and s on the stack, `ret 10h`).
// Identity: Ego_r.pdb / FableWin.pdb ?GFGetHermiteSplinePosition@@YIXAAVC3DVector@@ABV1@111M@Z; the pair is donor-ordered
//   GFDXGetHermiteSplinePosition (0x00A565C0 in retail: forwards to the D3DXVec3Hermite import 0x00BFEFDA with the same
//   argument order) then GFGetHermiteSplinePosition (this row, the plain-C++ twin). Only caller:
//   CCardinalSpline::GetSplinePosition 0x00A567A0. Argument order follows D3DXVec3Hermite (V1, T1, V2, T2, s).
// Constants: 0x0122DED4 = 3.0f, 0x0122DED8 = 1.0f.
// Shape: the four basis weights as named floats (s^2 / s^3 kept on the x87 stack), then one expression of inline
//   C3DVector operator* / operator+ temporaries; VC evaluates the right operand of each `+` first, hence the
//   t2, t1, v2, v1 product order.
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};

void __fastcall GFGetHermiteSplinePosition(C3DVector& result, const C3DVector& v1, const C3DVector& t1,
                                           const C3DVector& v2, const C3DVector& t2, float s)
{
    float s2 = s * s;
    float s3 = s * s2;
    float two_s3 = s3 + s3;
    float three_s2 = s2 * 3.0f;
    float h1 = two_s3 - three_s2 + 1.0f;
    float h2 = three_s2 - two_s3;
    float h3 = s3 - (s2 + s2) + s;
    float h4 = s3 - s2;
    result = v1 * h1 + v2 * h2 + t1 * h3 + t2 * h4;
}
