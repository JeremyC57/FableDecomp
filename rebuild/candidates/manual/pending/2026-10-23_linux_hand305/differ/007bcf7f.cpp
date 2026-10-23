// Grade: DIFFER(808v808) -- same size; the DrainLife (007bd845) residue: retail sums the shot length X*X first (ours Y*Y first; source operand order and the QFE compiler do not change it), one cross-product multiply operand swap, velocity as dir.X*speed (ours speed*dir.X), and frame slots 0xc lower than ours (retail packs the named fall/target vectors into the velocity slot -0x28 and a -0x58 temporary; ours gives them their own slots)
// NThingShot::CTCShotDefinitionMultiArrow::InitMovement() -- aim at the target (spread unless this is a multi-arrow child), face and launch the arrow, and for a lobbed arrow derive the gravity acceleration that lands it on the target; set the cone angle
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BCF7F, 808 bytes, ebp frame + `leave` (optimize("y", off) + ("s", on) TU).
// Callees: as 007bd845, with CThingShotDef::GetSubDef<multi-arrow def> 0x007BE7EA (catalog "GetSubDef<CAbilityDef>").
//   Floats 2.0f 0x0122DCDC, 1.0f 0x0122DED8, 0.0f 0x0122DEDC, 1/360 0x01238E00, 2*pi (double) 0x0124F2B8; fps 0x01375550.
// Layout: shot base PShotDef +0x38 (speed +0x3c), AccuracyPercentage +0x40; multi-arrow acceleration +0x9c, cos-cone +0xa8,
//   cone +0xac, MultiArrowShotType +0xb0 (Ego_r.pdb CTCShotDefinitionBase::MultiArrowShotType, +0x9c there, moved by the
//   derived layout); multi-arrow def max range +0x2c, spread +0x30, gravity +0x34, cone +0x38.
// Shape: velocity is a named local (its slot later holds the fall/target vectors); the fps*fps product is an int imul.
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
struct CMultiArrowDef : CDefBase {
    unsigned char _pad08[0x2c - 0x08];
    float         MaxRange;                                 // +0x2c
    float         MaxSpread;                                // +0x30
    float         Gravity;                                  // +0x34
    float         ConeAngle;                                // +0x38
};
struct CThingShotDef {
    unsigned char _pad[0x3c];
    float         Speed;                                    // +0x3c
    bool GetSubDef(CDefPointer<CMultiArrowDef>* out) const;  // 0x007BE7EA
};

CMultiArrowDef* FindRef_GetSubDef_0();
// Same-TU stand-in for the out-of-line GetSubDef (only writes the out pointer; the 0x007E5680 precedent).
#pragma auto_inline(off)
bool CThingShotDef::GetSubDef(CDefPointer<CMultiArrowDef>* out) const
{
    CMultiArrowDef* def = FindRef_GetSubDef_0();
    if (!def)
        return false;
    out->Data = def;
    return true;
}
#pragma auto_inline(on)

namespace NThingShot {

struct CTCShotDefinitionMultiArrow {
    void*                   __vftable;                      // +0x00
    CThing*                 ParentThing;                    // +0x04
    unsigned char           _pad08[0x38 - 0x08];
    const CThingShotDef*    PShotDef;                       // +0x38
    unsigned char           _pad3c[4];
    float                   AccuracyPercentage;             // +0x40
    unsigned char           _pad44[0x9c - 0x44];
    C3DVector               Acceleration;                   // +0x9c
    float                   CosConeAngle;                   // +0xa8
    float                   ConeAngle;                      // +0xac
    bool                    MultiArrowShotType;             // +0xb0
    C3DVector GetTargetPosition() const;                    // 0x007BEDD0
    CWorld* GetWorld();                                     // 0x00686A70
    void InitMovement();
};

void CTCShotDefinitionMultiArrow::InitMovement()
{
    CTCPhysicsBase* physics = ParentThing->Physics;
    C3DVector target = GetTargetPosition();
    C3DVector dir = target - physics->Position;
    float dist = dir.GetLength();
    if (!MultiArrowShotType) {
        long accuracy = (long)AccuracyPercentage;
        dir = RandomiseShotDirection(dir, accuracy, GetWorld()->GetRandomSeed());
    }
    dir.Normalise();
    C3DVector side = CrossProduct(C3DVector(dir.Y, -dir.X, 0.0f), dir);
    C3DVector up = CrossProduct(dir, side);
    side.Normalise();
    up.Normalise();
    physics->SetRHSet(CRightHandedSet(side, dir));
    float speed = PShotDef->Speed / g_GameFramesPerSecond;
    CDefPointer<CMultiArrowDef> def;
    PShotDef->GetSubDef(&def);
    if (!MultiArrowShotType) {
        if (dist > def->MaxRange)
            dist = def->MaxRange;
        dist = def->MaxSpread / def->MaxRange * dist;
        dir.Rotate(side, (GFFloatRandom(dist * 2.0f, GetWorld()->GetRandomSeed()) - dist) * 0.0027777778f);
        dir.Rotate(up, (GFFloatRandom(dist, GetWorld()->GetRandomSeed()) - dist) * 0.0027777778f);
    }
    C3DVector velocity = dir * speed;
    physics->SetVelocity(velocity);
    if (!MultiArrowShotType) {
        C3DVector fallVelocity = velocity * def->Gravity * (float)g_GameFramesPerSecond;
        C3DVector toTarget = (GetTargetPosition() - physics->Position - fallVelocity) * 2.0f;
        Acceleration = toTarget * (1.0f / ((float)(g_GameFramesPerSecond * g_GameFramesPerSecond) * def->Gravity * def->Gravity));
    }
    physics->Update(0);
    ConeAngle = def->ConeAngle * 0.0027777778f;
    CosConeAngle = (float)cos(ConeAngle * 6.2831854820251465);
}

}
