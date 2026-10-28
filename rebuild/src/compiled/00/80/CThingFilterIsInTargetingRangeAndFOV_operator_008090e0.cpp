// CThingFilter_IsInTargetingRangeAndFOV::operator()(CThing* thing) const -- a thing passes when it is (almost) at the targeting position, or within the targeting range for it and either inside the targeting cone or the targeter's live lock target
// Leaf: ??RCThingFilter_IsInTargetingRangeAndFOV
// Grade: RELOCATION_MATCH -- the lock-target test is IS_THING_ALIVE as `template<class T> bool IS_THING_ALIVE(T* const&)` (retail's own IS_THING_ALIVE<T*> signature, 0x0040DCE0), which tests the member in eax; the radius argument goes through an inline Targeter->GetPhysicsTC() accessor so all IsInFOV ctor arguments load before the pushes
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008090E0, length 190 bytes, `ret 4`, bool in al.
// Callees: CThing::GetPos @ 0x004C73D0, GetTargetingRangeForTarget(CThing*, CThing*, CThing*) @ 0x00802B60
//   (`__fastcall`, float), CThingFilter_IsInFOV::CThingFilter_IsInFOV(const C3DVector&, const C3DVector&, float, float, bool)
//   @ 0x006A7150 and CThingFilter_IsInFOV::operator()(CThing*) @ 0x006A7390 (engine_api.tsv).
// Global: g_epsilon_0129ba3c (1e-4f).
// Layout: CThingFilter_IsInTargetingRangeAndFOV (48 bytes, see the constructor 0x00809030): Position +0, FacingVector
//   +0xc, Targeter +0x18, PTargetLockThing +0x1c, PCarriedThing +0x24, TargetingFOV +0x28, AngleTest3D +0x2c; retail
//   CThing PhysicsTC +0x60, Dead +0x91 bit 0; CTCPhysicsBase Radius +0x34; CThingFilter_IsInFOV is 36 bytes (the frame).
// Shape: the delta is `thing->GetPos() - Position` through an inline operator-, its squared length the inline chain
//   `s = Z*Z; s += Y*Y; s += X*X;` kept in a named float local (parked in the dead argument slot); the cone test is a temporary
//   CThingFilter_IsInFOV called in place; the lock-target test is the inline IS_THING_ALIVE plus an identity compare.
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    float GetLengthSquared() const { float s = Z * Z; s += Y * Y; s += X * X; return s; }
};
inline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }

class CTCPhysicsBase { public:
    unsigned char _pad00[0x34];
    float Radius;                               // +0x34
};
class CThing {
public:
    unsigned char _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;
    CTCPhysicsBase* GetPhysicsTC() const { return PhysicsTC; }
    unsigned char _pad64[0x91 - 0x64];
    unsigned char Dead : 1;                     // +0x91 bit 0
    const C3DVector& GetPos() const;       // 0x004C73D0
};
template<class T> inline bool IS_THING_ALIVE(T* const& thing) { return thing && !thing->Dead; }

extern const float g_epsilon_0129ba3c;          // 0x0129BA3C (1e-4f)
float __fastcall GetTargetingRangeForTarget(CThing* target, CThing* targeter, CThing* carried);   // 0x00802B60

class CThingFilter_IsInFOV { public:
    C3DVector FromPos;                          // +0x00
    C3DVector Direction;                        // +0x0c
    float CosFOV;                               // +0x18
    float FOV;                                  // +0x1c
    bool AngleTest3D;                           // +0x20
    CThingFilter_IsInFOV(const C3DVector& fromPos, const C3DVector& direction, float fov, float radius, bool angleTest3D);   // 0x006A7150
    bool operator()(CThing* thing);             // 0x006A7390
};

class CTargetingDef;
struct CThingFilter_IsInTargetingRangeAndFOV {
    C3DVector Position;                         // +0x00
    C3DVector FacingVector;                     // +0x0c
    CThing* Targeter;                           // +0x18
    CThing* PTargetLockThing;                   // +0x1c
    CTargetingDef* TargetingDef;                // +0x20
    CThing* PCarriedThing;                      // +0x24
    float TargetingFOV;                         // +0x28
    bool AngleTest3D;                           // +0x2c
    bool operator()(CThing* thing) const;
};

bool CThingFilter_IsInTargetingRangeAndFOV::operator()(CThing* thing) const
{
    float distanceSquared = (thing->GetPos() - Position).GetLengthSquared();
    if (distanceSquared < g_epsilon_0129ba3c)
        return true;
    float range = GetTargetingRangeForTarget(thing, Targeter, PCarriedThing);
    if (distanceSquared < range * range) {
        if (CThingFilter_IsInFOV(Position, FacingVector, TargetingFOV, Targeter->GetPhysicsTC()->Radius, AngleTest3D)(thing)
            || (IS_THING_ALIVE(PTargetLockThing) && PTargetLockThing == thing))
            return true;
    }
    return false;
}