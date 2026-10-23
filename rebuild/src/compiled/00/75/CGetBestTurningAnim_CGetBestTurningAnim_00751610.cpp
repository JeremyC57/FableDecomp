// CGetBestTurningAnim::CGetBestTurningAnim(const C2DVector& facing, const C2DVector& target) -- search functor: no best anim yet; the rotation needed to turn from the facing to the target direction, in turns (acos of the clamped normalised dot product / 2pi), negative when the target lies clockwise
// Leaf: ??0CGetBestTurningAnim
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00751610, length 258 bytes, `ret 8`.
// Callees: C3DVector::Normalise @ 0x00A14440 (catalog "GetScaled" is propagated), _CIacos (MSVCR71 import thunk @ 0x00BFEBA2).
//   Float pool: 0x0122DEE0 = -1.0f, 0x0122DED8 = 1.0f, 0x0122DEDC = 0.0f, 0x01237F08 = 1/(2pi).
// Layout = Ego_r.pdb CGetBestTurningAnim (12 bytes): RotationAngle +0, BestMatchAngle +4, PBestMatchAnim +8.
// Shape: both directions become C3DVector locals (X/Y dword copies, Z = 0) normalised in place; the dot product is clamped
//   to [-1, 1] in the dead `facing` slot with immediate stores (if / else-if on a named float); the angle stays on the x87
//   stack across the side test (the dot with the facing's clockwise perpendicular) and is negated by `*= -1.0f`.
struct C2DVector { float X, Y; };
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    void Normalise();                                   // 0x00A14440
    float Dot(const C3DVector& o) const { return o.X * X + o.Y * Y + o.Z * Z; }
};
extern "C" double __cdecl acos(double x);
#pragma intrinsic(acos)
struct CAnimationEntry;
struct CGetBestTurningAnim {
    float            RotationAngle;                     // +0x00
    float            BestMatchAngle;                    // +0x04
    CAnimationEntry* PBestMatchAnim;                    // +0x08
    CGetBestTurningAnim(const C2DVector& facing, const C2DVector& target);
};

CGetBestTurningAnim::CGetBestTurningAnim(const C2DVector& facing, const C2DVector& target)
    : BestMatchAngle(0.0f)
{
    C3DVector from(facing.X, facing.Y, 0.0f);
    C3DVector to(target.X, target.Y, 0.0f);
    (void)from.Y;
    to.Z = 0.0f;
    from.Z = 0.0f;
    from.Normalise();
    to.Normalise();
    float cosAngle = from.X * to.X;
    cosAngle += from.Y * to.Y;
    cosAngle += to.Z * from.Z;
    if (cosAngle < -1.0f)
        cosAngle = -1.0f;
    else if (cosAngle > 1.0f)
        cosAngle = 1.0f;
    RotationAngle = (float)acos(cosAngle) * 0.15915494f;
    PBestMatchAnim = 0;
    if (C3DVector(from.Y, -from.X, 0.0f).Dot(to) < 0.0f)
        RotationAngle *= -1.0f;
}