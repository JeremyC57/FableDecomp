// NThingShot::CTCShotDefinitionDragonFireball::InitMovement() -- launch the fireball: aim from the shooter at the target position, or, while the target thing is alive, lead it (its last-frame motion, rotated about Z by a random signed angle and scaled by a random factor, over the flight time at the shot speed); scatter the aim by the accuracy, orient the body along it and give it the shot speed
// Grade: DIFFER(807v807) -- 1 grade; same length and 281/286 instructions equal modulo frame offsets: VC7.1 gives the target position the frame slot retail gives the delta (and swaps the speed / time words), the velocity's squared length sums Y,Z,X (retail X,Y,Z; six source orders identical) and the radius scaling loads the factor first
// Leaf: ?InitMovement@CTCShotDefinitionDragonFireball@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BCC40, 807 bytes, `ret`, vtable slot 33 (Ego_r.pdb
//   ?InitMovement@CTCShotDefinitionDragonFireball@NThingShot@@MAEXXZ). Size-optimised TU (`lea ebp,[esp-78h]`, `leave`).
// Callees: GetTargetPos 0x007BEDD0 (the unaligned TargetPos +0x51, hidden result), CThing::GetPosition 0x004C73D0,
//   CBaseIntelligentPointer::GetPItem 0x00A01B50, IS_THING_ALIVE 0x00436380 (__fastcall, a CBaseObject* const& bound to a
//   converted temporary), CTCBase::GetWorld 0x00686A70, CWorld::GetPRandomSeed 0x0049D810, GFFloatRandom 0x004978A0 and
//   GFRandom 0x004CDAE0 (__fastcall), C3DVector::Rotate 0x00A13C60 / Normalise 0x00A14440, the shot-accuracy scatter
//   0x007BEBE0 (__fastcall, hidden result in ecx), CRightHandedSet 0x00487050, the physics body's slot 32 (+0x80) and
//   slot 38 (+0x98), CTCPhysicsBase::ApplyAcceleration 0x006B0210, the float-to-int helper 0x00BFEA70.
// Constants: g_framesPerSecond @ 0x01375550 (int), the shared epsilon 1e-4f @ 0x0129BA3C (an extern const: retail squares
//   it at run time), 0.1f stored as 0x3DCCCCCD, -1.0f @ 0x0122DEE0, 0.0f @ 0x0122DEDC.
// Layout: Ego_r.pdb CTCShotDefinitionBase less 4 (12-byte retail vector): Thing +0x04, PTargetThing +0x20, PShotDef +0x38,
//   AccuracyPercentage +0x40; CThingShotDef less 4: Speed +0x3c, ShotOffsetAngleRange +0x64, ShotOffsetRadiusRange +0x6c
//   (CFloatRange = HighValue, LowValue); CTCPhysicsBase Position +0x0c, OldPosition +0x18.
// Shape: C3DVector has the engine's memberwise copy constructor; the range pick is a forced-inline member taking the
//   low and high values by value (the landed CTCWillLightning GetRandomInRange); the zero-motion fallback is written per
//   field; the accuracy is converted into a named local before the scatter call; the sign test is `== 0 ? -1 : 1`.
#pragma optimize("s", on)
#include <math.h>

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    __forceinline float GetLength() const { return (float)sqrt(Z * Z + Y * Y + X * X); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
    void Rotate(const C3DVector& axis, float angle);              // 0x00A13C60
    void Normalise();                                             // 0x00A14440
};
__forceinline C3DVector CrossProduct(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X);
}
__forceinline bool IsNearlyZero(const C3DVector& v, float tolerance) { return v.GetLengthSquared() <= tolerance * tolerance; }

struct CRightHandedSet {
    C3DVector Up;
    C3DVector Forward;
    CRightHandedSet(const C3DVector& up, const C3DVector& forward);   // 0x00487050
};

struct CFloatRange { float HighValue, LowValue; };

struct CTCPhysicsBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31();
    virtual void SetRHSet(const CRightHandedSet& set);           // slot 32 (+0x80)
    virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37();
    virtual void SetMoving(bool moving);                          // slot 38 (+0x98)
    unsigned char _pad04[0x0c - 0x04];
    C3DVector Position;                                           // +0x0c
    C3DVector OldPosition;                                        // +0x18
    void ApplyAcceleration(const C3DVector& acceleration);        // 0x006B0210
};

struct CBaseObject { };
struct CThing : CBaseObject {
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PPhysics;                                     // +0x60
    const C3DVector& GetPosition() const;                         // 0x004C73D0
};
bool __fastcall IS_THING_ALIVE(CBaseObject* const& object);            // 0x00436380

struct CBaseIntelligentPointer {
    void* _vtable;
    void* m_pItem;
    CThing* GetPItem() const;                                     // 0x00A01B50
};

struct CThingShotDef {
    unsigned char _pad00[0x3c];
    float         Speed;                                          // +0x3c
    unsigned char _pad40[0x64 - 0x40];
    CFloatRange   ShotOffsetAngleRange;                           // +0x64
    CFloatRange   ShotOffsetRadiusRange;                          // +0x6c
};
template <class T> struct CDefPointer {
    T* m_pDef;
    const T* operator->() const { return m_pDef; }
};

struct CWorld { unsigned long* GetPRandomSeed(); };              // 0x0049D810
float __fastcall GFFloatRandom(float range, unsigned long* seed);  // 0x004978A0
unsigned long __fastcall GFRandom(unsigned long range, unsigned long* seed);   // 0x004CDAE0
extern long g_FramesPerSecond;                                  // 0x01375550
extern const float g_Epsilon;                                   // 0x0129BA3C (1e-4f)

namespace NThingShot {

C3DVector __fastcall ApplyShotAccuracy(const C3DVector& direction, long accuracy, unsigned long* seed);   // 0x007BEBE0

class CTCShotDefinitionDragonFireball {
public:
    CThing*                     Thing;                           // +0x04
    unsigned char               _pad08[0x20 - 0x08];
    CBaseIntelligentPointer     PTargetThing;                    // +0x20
    unsigned char               _pad28[0x38 - 0x28];
    CDefPointer<CThingShotDef>  PShotDef;                        // +0x38
    unsigned char               _pad3c[0x40 - 0x3c];
    float                       AccuracyPercentage;              // +0x40
    CWorld* GetWorld() const;                                    // 0x00686A70
    C3DVector GetTargetPos() const;                              // 0x007BEDD0
    __forceinline float GetRandomInRange(float low, float high) const
    {
        return GFFloatRandom(high - low, GetWorld()->GetPRandomSeed()) + low;
    }
    virtual void InitMovement();
};

void CTCShotDefinitionDragonFireball::InitMovement()
{
    CTCPhysicsBase* physics = Thing->PPhysics;
    C3DVector target = GetTargetPos();
    C3DVector position = Thing->GetPosition();
    C3DVector direction = target - position;
    float speed = PShotDef->Speed / (float)g_FramesPerSecond;
    CThing* targetThing = PTargetThing.GetPItem();
    if (IS_THING_ALIVE(targetThing)) {
        C3DVector targetPos = targetThing->GetPosition();
        CTCPhysicsBase* targetPhysics = targetThing->PPhysics;
        C3DVector velocity = targetPhysics->Position - targetPhysics->OldPosition;
        C3DVector delta = targetPos - position;
        float time = 0.0f;
        if (g_Epsilon < PShotDef->Speed)
            time = delta.GetLength() / speed;
        if (!(velocity.GetLengthSquared() > g_Epsilon * g_Epsilon))
        {
            velocity.X = 0.0f;
            velocity.Y = 0.1f;
            velocity.Z = 0.0f;
        }
        float radius = GetRandomInRange(PShotDef->ShotOffsetRadiusRange.LowValue, PShotDef->ShotOffsetRadiusRange.HighValue);
        float angle = GetRandomInRange(PShotDef->ShotOffsetAngleRange.LowValue, PShotDef->ShotOffsetAngleRange.HighValue);
        float sign = GFRandom(1, GetWorld()->GetPRandomSeed()) == 0 ? -1.0f : 1.0f;
        velocity.Rotate(C3DVector(0.0f, 0.0f, 1.0f), sign * angle);
        direction = velocity * radius * time + targetPos - position;
    }
    long accuracy = (long)AccuracyPercentage;
    direction = ApplyShotAccuracy(direction, accuracy, GetWorld()->GetPRandomSeed());
    direction.Normalise();
    C3DVector up = CrossProduct(C3DVector(direction.Y, -direction.X, 0.0f), direction);
    up.Normalise();
    physics->SetRHSet(CRightHandedSet(up, direction));
    C3DVector acceleration = direction * speed;
    physics->ApplyAcceleration(acceleration);
    physics->SetMoving(false);
}

}
