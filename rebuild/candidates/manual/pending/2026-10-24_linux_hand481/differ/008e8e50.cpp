// NCreatureMode::GetStrafingAnimationName(const C3DVector& velocity, const C3DVector& facing, EMovementType) -- the strafe animation name for a creature moving with the velocity while facing the direction: STRAFE_IDLE when slower than 0.005, otherwise forward/back when the flattened movement lies within 45 degrees of the flattened facing, else left/right by the sign of the cross product's Z
// Grade: DIFFER(409v405) -- x87 allocation of the speed only, 3 instructions: retail moves the fsqrt result under the dead velocity loads and keeps it (`fstp st(3) ; fstp st(0) ; fstp st(0) ; fst [esp+4] ; fcomp eps`), ours stores it at once and reloads it (`fstp [esp+4] ; fstp st(0) x3 ; fld [esp+4] ; fcomp eps`); everything else (copy schedule, dot/cross term order, 45-degree test, string selects, return copy) matches. ~50 diagnostic variants (statement orders, magnitude forms, assignment-in-condition, /Oa /Ow /G5-/G7 /Ox, QFE toolset) left the store unchanged; same family as 0x008F5970.
// Leaf: ?GetStrafingAnimationName@NCreatureMode@@YI?AVCCharString@@ABVC3DVector@@0W4EMovementType@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E8E50, 416 bytes incl. int3 padding, /Gr `__fastcall` (hidden result in
//   ecx, velocity in edx, facing and the unused EMovementType on the stack), `ret 8`.
// Identity: catalog "GetStrafeAnimationName" (_global) is the PDB name set's
//   `?GetStrafingAnimationName@NCreatureMode@@YI?AVCCharString@@ABVC3DVector@@0W4EMovementType@@@Z` (an overload takes a
//   CThing); callers 0x008E9068 and 0x008E9127 (the CThing overload's neighbourhood).
// Callees: C3DVector::AccurateNormalise() 0x00A14440 (returns *this; catalog "GetScaled" is propagated), CCharString
//   ctor(const char*, long) 0x0099EBF0, operator=(const char*) 0x0099EFE0, copy ctor 0x0099EC30, dtor 0x0099EAE0.
// Constants: 0x0129BA3C the 1e-4f epsilon (an extern float: squared at run time), 0x01230B20 0.005f, 0x0122DED8 1.0f,
//   0x0122DEDC 0.0f, 0x01244480 2.0 (double, square-rooted at run time); strings 0x01287C64 "STRAFE_IDLE", 0x01287C4C
//   "STRAFE_MOVEMENT_BACK", 0x01287C34 "..._FORWARD", 0x01287C1C "..._LEFT", 0x01287C04 "..._RIGHT".
// Shape: both vectors copied through a user memberwise copy constructor, facing's Z zeroed before the direction's (that
//   order fixes the store schedule and the dot product's term order: Y, X, then the direction-first Z term); the speed is
//   the full 3D velocity magnitude; the cross product is a named C3DVector of which only Z is stored; the 45-degree test is
//   `1.0f / (float)sqrt(2.0)` (float divide: the dword 1.0f and the fxch pair); the name is returned by copy (no NRVO).
class CCharString {
public:
    void* PStringData;                                      // +0x00
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
    CCharString& operator=(const char* text);               // 0x0099EFE0
};

extern "C" double __cdecl sqrt(double);
extern "C" double __cdecl fabs(double);
#pragma intrinsic(sqrt, fabs)

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    float GetMagnitude() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
    float GetMagnitudeSquared() const { return X * X + Y * Y + Z * Z; }
    C3DVector& AccurateNormalise();                          // 0x00A14440
    float Dot(const C3DVector& v) const { return X * v.X + Y * v.Y + Z * v.Z; }
    C3DVector CrossProduct(const C3DVector& v) const { return C3DVector(Y * v.Z - Z * v.Y, Z * v.X - X * v.Z, X * v.Y - Y * v.X); }
    C3DVector& operator*=(float s) { X *= s; Y *= s; Z *= s; return *this; }
};

extern const float g_epsilon_0129ba3c;                      // 0x0129BA3C (1e-4f)

enum EMovementType { MOVEMENT_TYPE_0 };

namespace NCreatureMode {

CCharString __fastcall GetStrafingAnimationName(const C3DVector& velocity, const C3DVector& facing, EMovementType)
{
    C3DVector direction(velocity);
    C3DVector facing_2d(facing);
    facing_2d.Z = 0.0f;
    direction.Z = 0.0f;
    float speed = velocity.GetMagnitude();
    if (speed > g_epsilon_0129ba3c)
        direction *= 1.0f / speed;
    if (facing_2d.GetMagnitudeSquared() > g_epsilon_0129ba3c * g_epsilon_0129ba3c)
        facing_2d.AccurateNormalise();
    C3DVector cross = direction.CrossProduct(facing_2d);
    float forward = facing_2d.Dot(direction);
    CCharString name("STRAFE_IDLE");
    if (speed > 0.005f) {
        if (fabs(forward) > 1.0f / (float)sqrt(2.0))
            name = forward < 0.0f ? "STRAFE_MOVEMENT_BACK" : "STRAFE_MOVEMENT_FORWARD";
        else
            name = cross.Z < 0.0f ? "STRAFE_MOVEMENT_LEFT" : "STRAFE_MOVEMENT_RIGHT";
    }
    return name;
}

}
