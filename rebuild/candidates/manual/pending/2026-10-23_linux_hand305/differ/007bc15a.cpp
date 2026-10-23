// NThingShot::CTCShotDefinitionMultiArrow::UpdateMovement() -- home each arrow in on a live target (turning at most a fixed angle per frame, at the shot def's per-frame speed); without a live target let it fall under its gravity
// Grade: DIFFER(571v560) -- same residue as the sibling 007BCA30: everything up to the fire-point subtraction matches; retail stores toTarget X and Y and keeps Z (ours keeps X), and the second fast Normalise's length-squared reuses the dead target slot [ebp-0xc] (ours [ebp-8]); a component-local-free Normalise gives identical code
// Leaf: UpdateMovement
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BC15A, 560 bytes, ebp frame + `leave` (optimize("y", off) + ("s", on) TU).
// Callees: GetPItem 0x00A01B50, IS_THING_ALIVE<CThing*> 0x00436380, GetCreatureFireAtPos 0x007BEDF0 (landed),
//   C3DVector::Rotate 0x00A13C60, physics SetVelocity 0x0066F9B0 / ApplyAcceleration 0x006B0210.
// Layout: shot base PTargetThing +0x20, PShotDef +0x38 (speed +0x3c); MultiArrow gravity vector +0x9c, max cos turn +0xa8,
//   max turn angle +0xac, stop-homing flag +0xb0 (names descriptive); physics +0x60 (position +0x0c, velocity +0x28).
#pragma optimize("y", off)
#pragma optimize("s", on)
extern unsigned long g_InvSqrtMantissaTable[128];          // 0x013CE558

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    __forceinline C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    __forceinline void Normalise()
    {
        float x = X;
        float y = Y;
        float z = Z;
        float lengthSquared = x * x + y * y + z * z;
        unsigned long bits = *(unsigned long*)&lengthSquared;
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = (3.0f - r * r * lengthSquared) * r * 0.5f;
        X *= r;
        Y *= r;
        Z *= r;
    }
    __forceinline C3DVector& operator*=(float f) { X *= f; Y *= f; Z *= f; return *this; }
    void Rotate(const C3DVector& axis, float angle);       // 0x00A13C60
};
__forceinline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
__forceinline float DotProduct(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
__forceinline C3DVector CrossProduct(const C3DVector& a, const C3DVector& b)
{
    C3DVector r;
    r.X = a.Y * b.Z - a.Z * b.Y;
    r.Y = a.Z * b.X - a.X * b.Z;
    r.Z = a.X * b.Y - a.Y * b.X;
    return r;
}

struct CTCPhysicsBase {
    unsigned char _pad00[0x0c];
    C3DVector     Position;                                 // +0x0c
    unsigned char _pad18[0x10];
    C3DVector     Velocity;                                 // +0x28
    void SetVelocity(const C3DVector& velocity);            // 0x0066F9B0
    void ApplyAcceleration(const C3DVector& acceleration);  // 0x006B0210
};
struct CThing {
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* Physics;                                // +0x60
};
C3DVector __fastcall GetCreatureFireAtPos(CThing* creature);   // 0x007BEDF0
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                                        // +4
    CThing* GetPItem() const;                               // 0x00A01B50
};
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);

struct CThingShotDef {
    unsigned char _pad[0x3c];
    float         Speed;                                    // +0x3c
};
extern long g_GameFramesPerSecond;                          // 0x01375550

namespace NThingShot {

struct CTCShotDefinitionBase {
    void*                   __vftable;                      // +0x00
    CThing*                 ParentThing;                    // +0x04
    unsigned char           _pad08[0x20 - 0x08];
    CBaseIntelligentPointer PTargetThing;                   // +0x20
    unsigned char           _pad28[0x38 - 0x28];
    const CThingShotDef*    PShotDef;                       // +0x38
    unsigned char           _pad3c[0x9c - 0x3c];
};
struct CTCShotDefinitionMultiArrow : CTCShotDefinitionBase {
    C3DVector               Gravity;                        // +0x9c
    float                   MaxCosAngleToTurn;              // +0xa8
    float                   MaxAngleToTurn;                 // +0xac
    bool                    StopHoming;                     // +0xb0
    void UpdateMovement();
};

void CTCShotDefinitionMultiArrow::UpdateMovement()
{
    CTCPhysicsBase* physics = ParentThing->Physics;
    CThing* target = PTargetThing.GetPItem();
    if (IS_THING_ALIVE(target)) {
        if (!StopHoming) {
            C3DVector velocity = physics->Velocity;
            velocity.Normalise();
            C3DVector position = physics->Position;
            C3DVector fireAt = GetCreatureFireAtPos(target);
            C3DVector toTarget = fireAt - position;
            toTarget.Normalise();
            if (DotProduct(velocity, toTarget) < MaxCosAngleToTurn) {
                C3DVector axis = CrossProduct(toTarget, velocity);
                velocity.Rotate(axis, MaxAngleToTurn);
            } else {
                velocity = toTarget;
            }
            velocity *= PShotDef->Speed / g_GameFramesPerSecond;
            physics->SetVelocity(velocity);
        }
    } else {
        physics->ApplyAcceleration(Gravity);
    }
}

}
