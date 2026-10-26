// NCameraModes::CBase::UpdateFixLookDirection(const C3DVector& pos, const C3DVector& target, C3DVector& lookDir, float angle) -- flatten the look direction onto the XY plane and tilt it back about its horizontal perpendicular by at most the given turn fraction, keeping the original up/down sense
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084CDF0, length 321 bytes (`ret 0x10`; `this`, pos and target are unused).
// Callees: C3DVector::AccurateNormalise @ 0x00A14440 (x2; manifest "GetScaled" is propagated), C3DVector::Rotate(const C3DVector&
//          axis, float angle) @ 0x00A13C60, _CIacos @ 0x00BFEBA2 (acos intrinsic on the x87 stack), fcos inline; constants
//          0.0f @ 0x0122DEDC, 1.0f @ 0x0122DED8, -1.0f @ 0x0122DEE0, 2*pi (double) @ 0x0124F2B8, 1/(2*pi) @ 0x01237F08.
// Layout: C3DVector {X, Y, Z} per the landed NCameraModes::CZTargetNew::OnInit 0x00872A20 model (copy ctor, inline
//          GetPerpendicularXY).
// Shape: the up/down sense is a bool local (Z > 0) kept in bl; cos(angle * 2pi) is homed in the dead angle slot and the dot
//          product in the dead lookDir slot; the dot is clamped below by that cosine and then to [-1, 1]; the flattened copy is
//          rotated about its XY perpendicular by acos(dot) / (2pi), negated when the direction pointed up, and written back.
extern "C" double __cdecl cos(double);
extern "C" double __cdecl acos(double);
#pragma intrinsic(cos, acos)

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector& AccurateNormalise();                                        // 0x00A14440 (Ego_r ?AccurateNormalise@C3DVector@@QAEAAV1@XZ)
    void Rotate(const C3DVector& axis, float angle);         // 0x00A13C60
    C3DVector GetPerpendicularXY() const { return C3DVector(Y, -X, 0.0f); }   // inline (name descriptive)
};

namespace NCameraModes {
struct CBase {
    void UpdateFixLookDirection(const C3DVector& pos, const C3DVector& target, C3DVector& lookDir, float angle);
};

void CBase::UpdateFixLookDirection(const C3DVector& pos, const C3DVector& target, C3DVector& lookDir, float angle)
{
    bool lookingUp = lookDir.Z > 0.0f;
    float minCos = (float)cos(angle * 6.2831854820251465);
    C3DVector dir = lookDir;
    dir.AccurateNormalise();
    C3DVector flat(dir.X, dir.Y, 0.0f);
    flat.AccurateNormalise();
    C3DVector axis = flat.GetPerpendicularXY();
    float dot = dir.Z * flat.Z + flat.X * dir.X + flat.Y * dir.Y;
    if (dot < minCos)
        dot = minCos;
    if (dot < -1.0f)
        dot = -1.0f;
    else if (dot > 1.0f)
        dot = 1.0f;
    float sign = lookingUp ? -1.0f : 1.0f;
    float turn = (float)acos(dot) * 0.15915493667125702f;
    flat.Rotate(axis, turn * sign);
    lookDir = flat;
}
}