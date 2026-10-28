// CThingFilter_IsEvadeExplosion::IsTravellingAway(const C3DVector& a, const C3DVector& b) const -- (anonymous namespace) whether two directions are within about 25 degrees of each other (their dot product exceeds cos(0.436 rad))
// Leaf: ?IsTravellingAway@CThingFilter_IsEvadeExplosion@?A0x
// Retail: Fable.exe @ 0x0080C140, 58 bytes, thiscall `ret 8` (`this` unused); result returned in eax (`mov eax,1`).
// Identity (proven): Ego_r.pdb names ?IsTravellingAway@CThingFilter_IsEvadeExplosion@?A0x41ace303@@QBE_NABVC3DVector@@0@Z,
//   in the same anonymous namespace (0x41ace303) as the landed GetExplosionEffectInterpolate 0x0080C190 beside it; the only
//   caller is the filter's operator() 0x0080DC90 (`??RCThingFilter_IsEvadeExplosion@?A0x41ace303@@QBE_NABVCThing@@@Z`),
//   which passes `this` in ecx and two C3DVector addresses. The catalog label "CCompareTargetedPositions::operator()" is
//   propagated (that PDB comparator is a different anonymous-namespace sort functor).
// Body: b.Dot(a) (inline C3DVector::Dot, FableWin.pdb ?Dot@C3DVector@@QBE?BMABV1@@Z; `fld [a] ; fmul [b]` is b * a) compared
//   with cos(0.43633232f) as a double (the float literal widened: qword 0x3FDBECDE60000000 at 0x01272318), the x87
//   `fcos` intrinsic (`#pragma intrinsic(cos)`; <math.h>'s C++ float overload would add an `fxch` pair).
// Shape: `return dot > cos(...)` gives retail's `mov eax,1 / xor eax,eax` bool tail; an if/return gives `mov al,1`.
extern "C" double __cdecl cos(double);
#pragma intrinsic(cos)
class C3DVector {
public:
    float X, Y, Z;
    const float Dot(const C3DVector& other) const { return X * other.X + Y * other.Y + Z * other.Z; }
};
namespace {
class CThingFilter_IsEvadeExplosion {
public:
    C3DVector* StartPos;                                 // +0x00
    C3DVector* EndPos;                                   // +0x04
    bool IsTravellingAway(const C3DVector& a, const C3DVector& b) const;
};
bool CThingFilter_IsEvadeExplosion::IsTravellingAway(const C3DVector& a, const C3DVector& b) const
{
    return b.Dot(a) > cos(0.43633232f);
}
}
// A small external caller keeps the anonymous-namespace member emitted (GOTCHAS chunk 488).
#pragma auto_inline(off)
bool IsTravellingAway_0080c140(const C3DVector& a, const C3DVector& b)
{
    CThingFilter_IsEvadeExplosion filter;
    return filter.IsTravellingAway(a, b);
}
#pragma auto_inline(on)