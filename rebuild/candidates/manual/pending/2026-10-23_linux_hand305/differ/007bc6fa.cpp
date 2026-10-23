// Grade: DIFFER(514v514) -- one instruction scheduled differently: retail reloads the spilled physics pointer (`mov esi,[ebp-8]`) before the three `fstp st(0)` pops at the end of the lead block, ours after the second pop (QFE compiler, named scale/offset/time variants identical)
// NThingShot::CTCShotDefinitionProjectilePredictive::InitMovement() -- aim the shot ahead of a live moving target, then face and launch it
// Leaf: InitMovement
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BC6FA, 514 bytes, ebp frame ending in `leave` (optimize("y", off) + ("s", on) TU).
// Callees: GetTargetPosition 0x007BEDD0 (catalog "CTCPhysicsRigidBody::GetFacingVector", folded body reading this+0x51),
//   CBaseIntelligentPointer::GetPItem 0x00A01B50, IS_THING_ALIVE<CThing*> 0x00436380 / 0x0040DCE0 (two folded
//   instantiations, `CThing* const&` in ecx), CThing::GetPosition 0x004C73D0, C3DVector::Normalise 0x00A14440 (catalog
//   "GetScaled"), CRightHandedSet(fwd, up) 0x00487050, physics vslot 32 (+0x80) / 38 (+0x98), CTCPhysicsBase::
//   ApplyAcceleration 0x006B0210. Floats 3.0f 0x0122DED4, 0.01f 0x01230A5C, 0.0f 0x0122DEDC; g_GameFramesPerSecond 0x01375550.
// Layout: shot def PTarget +0x20 / PShooter +0x28 (CIntelligentPointer<CThing>), PShotDef +0x38 (speed +0x3c, as the landed
//   GetInitialVelocity 0x007BEFB0), accuracy percent float +0x40; CThing physics +0x60 (position +0x0c, old position +0x18).
// Shape: free operator-(a, b) evaluates b first; Distance(from, to) is called (shooter, target) so the target pointer is read
//   first; IS_THING_ALIVE takes the GetPItem temporary by const reference (its slot is reused by the __ftol2 int); the launch
//   velocity is a named function-scope local (its own frame slot).
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <math.h>
#pragma intrinsic(sqrt)
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    __forceinline C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    __forceinline C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    __forceinline C3DVector& operator+=(const C3DVector& o) { X += o.X; Y += o.Y; Z += o.Z; return *this; }
    __forceinline float GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
    void Normalise();                                       // 0x00A14440
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
    unsigned char _pad04[8];
    C3DVector Position;                                     // +0x0c
    C3DVector OldPosition;                                  // +0x18
    void ApplyAcceleration(const C3DVector& acceleration);  // 0x006B0210
};
struct CThing {
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* Physics;                                // +0x60
    const C3DVector& GetPosition() const;                   // 0x004C73D0
};
__forceinline float Distance(CThing* from, CThing* to) { return (to->GetPosition() - from->GetPosition()).GetLength(); }
bool __fastcall IS_THING_ALIVE(CThing* const& thing);       // 0x00436380 / 0x0040DCE0
struct CBaseIntelligentPointer {
    void* __vftable;
    void* PData;
    CThing* GetPItem() const;                               // 0x00A01B50
};
struct CThingShotDef {
    unsigned char _pad[0x3c];
    float         Speed;                                    // +0x3c
};
extern long g_GameFramesPerSecond;                          // 0x01375550

namespace NThingShot {

struct CTCShotDefinitionProjectilePredictive {
    void*                   __vftable;                      // +0x00
    CThing*                 ParentThing;                    // +0x04
    unsigned char           _pad08[0x18];
    CBaseIntelligentPointer PTarget;                        // +0x20
    CBaseIntelligentPointer PShooter;                       // +0x28
    unsigned char           _pad30[8];
    const CThingShotDef*    PShotDef;                       // +0x38
    unsigned char           _pad3c[4];
    float                   Accuracy;                       // +0x40
    C3DVector GetTargetPosition() const;                    // 0x007BEDD0
    void InitMovement();
};

void CTCShotDefinitionProjectilePredictive::InitMovement()
{
    CTCPhysicsBase* physics = ParentThing->Physics;
    C3DVector target = GetTargetPosition();
    if (IS_THING_ALIVE(PTarget.GetPItem()) && IS_THING_ALIVE(PShooter.GetPItem())) {
        C3DVector motion = PTarget.GetPItem()->GetPosition() - PTarget.GetPItem()->Physics->OldPosition;
        float time = Distance(PShooter.GetPItem(), PTarget.GetPItem()) / PShotDef->Speed * 3.0f;
        C3DVector lead = motion * time;
        target += lead * ((long)Accuracy * 0.01f);
    }
    C3DVector dir = target - physics->Position;
    dir.Normalise();
    C3DVector side = CrossProduct(C3DVector(dir.Y, -dir.X, 0.0f), dir);
    side.Normalise();
    physics->SetRHSet(CRightHandedSet(side, dir));
    C3DVector velocity = dir * (PShotDef->Speed / g_GameFramesPerSecond);
    physics->ApplyAcceleration(velocity);
    physics->Update(0);
}

}
