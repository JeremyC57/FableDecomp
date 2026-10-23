// Grade: DIFFER(642v642) -- same size and frame; retail sums the shot length as X*X first (ours Y*Y first), loads `lea ecx,[ebp-0x20]` mid-way through the second cross product, swaps one cross-product multiply operand pair, computes the velocity as dir.X*speed (ours speed*dir.X), and shares the RandomiseShotDirection temporary's slot with the velocity temporary (-0x44; ours -0x50)
// NThingShot::CTCShotDefinitionDrainLife::InitMovement() -- aim the drain-life bolt at its target with accuracy spread, face it, add a random range-scaled wobble about both side axes, launch it and set its cone angle and lifetime
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BD845, 642 bytes, ebp frame + `leave` (optimize("y", off) + ("s", on) TU).
// Callees: GetTargetPosition 0x007BEDD0, physics vslots +0xac / +0x80 / +0x98, CTCBase::GetWorld 0x00686A70, the world's
//   random-seed getter 0x0049D810 (catalog "DoSizeof", folded), __ftol2 0x00BFEA70, RandomiseShotDirection 0x007BEBE0
//   (__fastcall, hidden return in ecx), C3DVector::Normalise 0x00A14440 / Rotate 0x00A13C60, CRightHandedSet 0x00487050,
//   CThingShotDef::GetSubDef<drain-life def> 0x007BE92E (catalog "GetSubDef<CAbilityDef>"), GFFloatRandom 0x004978A0,
//   physics SetVelocity 0x0066F9B0. Floats 0.0f 0x0122DEDC, 1/360 0x01238E00, 2*pi (double) 0x0124F2B8; fps 0x01375550.
// Layout: shot base PShotDef +0x38 (lifetime +0x3c), AccuracyPercentage +0x40; drain-life cos-cone +0x9c, cone +0xa0,
//   lifetime frames +0xa8; drain-life def max range +0x28, max spread +0x2c, cone +0x30, speed +0x34, lifetime variance +0x38.
// Shape: a same-TU stand-in GetSubDef body lets VC7.1 keep the sub-def in esi and reuse its slot for the speed (the
//   retail [ebp-8] triple use); the accuracy is a named long local (ftol before the seed call); user copy constructor.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <math.h>
#pragma intrinsic(sqrt, cos)
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    __forceinline C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    __forceinline C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    __forceinline C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    __forceinline C3DVector& operator+=(const C3DVector& o) { X += o.X; Y += o.Y; Z += o.Z; return *this; }
    __forceinline float GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
    void Normalise();                                       // 0x00A14440
    void Rotate(const C3DVector& axis, float angle);        // 0x00A13C60
};
__forceinline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
__forceinline C3DVector CrossProduct(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X);
}
struct CRightHandedSet {
    C3DVector Forward, Up;
    CRightHandedSet(const C3DVector& forward, const C3DVector& up);   // 0x00487050
};

struct CTCPhysicsBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31();
    virtual void SetRHSet(const CRightHandedSet& rh);       // slot 32 (+0x80)
    virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37();
    virtual void Update(long flags);                        // slot 38 (+0x98)
    virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42();
    virtual void Stop(long flags);                          // slot 43 (+0xac)
    unsigned char _pad04[8];
    C3DVector Position;                                     // +0x0c
    C3DVector OldPosition;                                  // +0x18
    void SetVelocity(const C3DVector& velocity);            // 0x0066F9B0
};
struct CThing {
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* Physics;                                // +0x60
    const C3DVector& GetPosition() const;                   // 0x004C73D0
};
extern long g_GameFramesPerSecond;                          // 0x01375550
struct CWorld { unsigned long* GetRandomSeed(); };          // 0x0049D810
C3DVector __fastcall RandomiseShotDirection(const C3DVector& dir, long accuracy, unsigned long* seed);   // 0x007BEBE0
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                                 // slot 1
    long RefCount;                                          // +0x04
};
template <class T> struct CDefPointer {
    T* Data;
    CDefPointer() : Data(0) { }
    ~CDefPointer() { if (--Data->RefCount == 0) Data->Destroy(); }
    T* operator->() const { return Data; }
};
struct CDrainLifeDef : CDefBase {
    unsigned char _pad08[0x28 - 0x08];
    float         MaxRange;                                 // +0x28
    float         MaxSpread;                                // +0x2c
    float         ConeAngle;                                // +0x30
    float         Speed;                                    // +0x34
    float         LifeVariance;                             // +0x38
};
struct CThingShotDef {
    unsigned char _pad[0x3c];
    float         Lifetime;                                 // +0x3c
    bool GetSubDef(CDefPointer<CDrainLifeDef>* out) const;  // 0x007BE92E
};

CDrainLifeDef* FindRef_GetSubDef_0();
// Same-TU stand-in for the out-of-line GetSubDef (only writes the out pointer; the 0x007E5680 precedent).
#pragma auto_inline(off)
bool CThingShotDef::GetSubDef(CDefPointer<CDrainLifeDef>* out) const
{
    CDrainLifeDef* def = FindRef_GetSubDef_0();
    if (!def)
        return false;
    out->Data = def;
    return true;
}
#pragma auto_inline(on)

namespace NThingShot {

struct CTCShotDefinitionDrainLife {
    void*                   __vftable;                      // +0x00
    CThing*                 ParentThing;                    // +0x04
    unsigned char           _pad08[0x38 - 0x08];
    const CThingShotDef*    PShotDef;                       // +0x38
    unsigned char           _pad3c[4];
    float                   AccuracyPercentage;             // +0x40
    unsigned char           _pad44[0x9c - 0x44];
    float                   CosConeAngle;                   // +0x9c
    float                   ConeAngle;                      // +0xa0
    unsigned char           _pada4[4];
    float                   LifeFrames;                     // +0xa8
    C3DVector GetTargetPosition() const;                    // 0x007BEDD0
    CWorld* GetWorld();                                     // 0x00686A70
    void InitMovement();
};

void CTCShotDefinitionDrainLife::InitMovement()
{
    CTCPhysicsBase* physics = ParentThing->Physics;
    C3DVector target = GetTargetPosition();
    physics->Stop(0);
    C3DVector dir = target - physics->Position;
    float dist = dir.GetLength();
    long accuracy = (long)AccuracyPercentage;
    dir = RandomiseShotDirection(dir, accuracy, GetWorld()->GetRandomSeed());
    dir.Normalise();
    C3DVector side = CrossProduct(C3DVector(dir.Y, -dir.X, 0.0f), dir);
    C3DVector up = CrossProduct(dir, side);
    side.Normalise();
    up.Normalise();
    physics->SetRHSet(CRightHandedSet(side, dir));
    CDefPointer<CDrainLifeDef> def;
    PShotDef->GetSubDef(&def);
    float speed = def->Speed / g_GameFramesPerSecond;
    if (dist > def->MaxRange)
        dist = def->MaxRange;
    dist = def->MaxSpread / def->MaxRange * dist;
    dir.Rotate(side, (GFFloatRandom(dist * 2.0f, GetWorld()->GetRandomSeed()) - dist) * 0.0027777778f);
    dir.Rotate(up, (GFFloatRandom(dist, GetWorld()->GetRandomSeed()) - dist) * 0.0027777778f);
    C3DVector velocity = dir * speed;
    physics->SetVelocity(velocity);
    physics->Update(0);
    ConeAngle = def->ConeAngle * 0.0027777778f;
    CosConeAngle = (float)cos(ConeAngle * 6.2831854820251465);
    LifeFrames = PShotDef->Lifetime / g_GameFramesPerSecond;
    float variance = def->LifeVariance / g_GameFramesPerSecond;
    LifeFrames += GFFloatRandom(variance, GetWorld()->GetRandomSeed());
}

}
