// NCameraModes::CProjectileWeapon::GetLookDirectionFromParams(float phi, float heightParam) const -- the projectile camera's look direction for an orbit angle and height: the horizontal look direction pitched (about its horizontal perpendicular) by the difference between the pitch towards the observed thing's aim point (1.6 above its position) from the camera position and the look direction's own pitch, both as wrapped fractions of a turn
// Leaf: ?GetLookDirectionFromParams@CProjectileWeapon@NCameraModes@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00878DB0, length 408 bytes (`ret 0Ch`: hidden C3DVector result, phi,
// heightParam). Ego_r.pdb `?GetLookDirectionFromParams@CProjectileWeapon@NCameraModes@@ABE?AVC3DVector@@MM@Z` (private const);
// the catalog label "CProjectileWeapon_CalculatePitch" is descriptive only. Callers: four sites in CProjectileWeapon's TU.
// Callees: CProjectileWeapon::GetPosFromParams(float, float) const 0x00878BE0, NCameraModes::CBase::GetObservedThing() const
//          0x0084F1B0, CThing::GetPosition() const 0x004C73D0, CProjectileWeapon::GetLookDirectionXYFromParams(float, float)
//          const 0x00878B60 (landed), _CIfmod 0x00BFEB28 (the fmod intrinsic), C3DVector::Rotate(const C3DVector&, float)
//          0x00A13C60.
// Pools: 1.6f 0x012777F0, 1/(2*pi) 0x01237F08, 0.0f 0x0122DEDC, 1.0f 0x0122DED8, 1.0 (double) 0x01237F00.
// Shape: the target offset is `C3DVector(thing.X, thing.Y, thing.Z + 1.6f) - camera` (the Z sum parks in the heightParam
// slot during the next call's pushes); each pitch is the landed facing-fraction shape (atan2 * 1/(2*pi) as a float, then
// `if (f < 0 || f >= 1) { f = fmod(f, 1.0); if (f < 0) f += 1; }`) in a __forceinline helper. The horizontal length must be
// a float local (`xy`) for retail's `fxch st(1) ; fxch st(1)` before fpatan. The rotation axis is built through a side
// vector `(look.Y, -look.X, 0)` negated memberwise (retail's double fchs on look.X and the +0.0f Z store). The result is
// returned by the memberwise user copy constructor (X through the x87, Y/Z as dwords).
extern "C" double __cdecl atan2(double y, double x);
extern "C" double __cdecl fmod(double x, double y);
extern "C" double __cdecl sqrt(double x);
#pragma intrinsic(atan2, fmod, sqrt)

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator-(const C3DVector& r) const { return C3DVector(X - r.X, Y - r.Y, Z - r.Z); }
    void Rotate(const C3DVector& axis, float angle);          // 0x00A13C60
};
class CThing {
public:
    const C3DVector& GetPosition() const;                     // 0x004C73D0
};
__forceinline float PitchFraction(const C3DVector& v)
{
    float xy = (float)sqrt(v.X * v.X + v.Y * v.Y);
    float fraction = (float)atan2(v.Z, xy) * 0.15915493667125702f;
    if (fraction < 0.0f || fraction >= 1.0f) {
        fraction = (float)fmod(fraction, 1.0);
        if (fraction < 0.0f)
            fraction += 1.0f;
    }
    return fraction;
}
namespace NCameraModes {
class CBase {
public:
    virtual void v00();
    const CThing* GetObservedThing() const;                   // 0x0084F1B0
};
class CProjectileWeapon : public CBase {
public:
    C3DVector GetPosFromParams(float phi, float heightParam) const;               // 0x00878BE0
    C3DVector GetLookDirectionXYFromParams(float phi, float heightParam) const;   // 0x00878B60
    C3DVector GetLookDirectionFromParams(float phi, float heightParam) const;
};
}

C3DVector NCameraModes::CProjectileWeapon::GetLookDirectionFromParams(float phi, float heightParam) const
{
    C3DVector camera = GetPosFromParams(phi, heightParam);
    const C3DVector& thing = GetObservedThing()->GetPosition();
    C3DVector toTarget = C3DVector(thing.X, thing.Y, thing.Z + 1.6f) - camera;
    C3DVector look = GetLookDirectionXYFromParams(phi, heightParam);
    C3DVector side(look.Y, -look.X, 0.0f);
    C3DVector axis(-side.X, -side.Y, side.Z);
    float targetPitch = PitchFraction(toTarget);
    float lookPitch = PitchFraction(look);
    look.Rotate(axis, targetPitch - lookPitch);
    return look;
}