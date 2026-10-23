// Grade: DIFFER(539v528) -- frame slots, the first fast Normalise and the turn/rotate tail match; the fire-point subtraction keeps X on the x87 stack (retail stores X and Y, keeps Z) and the second Normalise's length-squared lands in [ebp-8] (retail reuses the dead target slot [ebp-0xc])
// NThingShot::CTCShotDefinitionDrainLife::UpdateMovement() -- home in on a live target: turn the shot's velocity towards the target's fire point by at most MaxAngleToTurn, then set it at Speed
// Leaf: UpdateMovement
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BCA30, 528 bytes, ebp frame + `leave` (optimize("y", off) + ("s", on) TU).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, IS_THING_ALIVE<CThing*> 0x00436380, GetCreatureFireAtPos
//   0x007BEDF0 (__fastcall, hidden return in ecx, creature in edx), C3DVector::Rotate(axis, angle) 0x00A13C60,
//   the physics' SetVelocity 0x0066F9B0 (catalog "SetFacingAnglesFromRHSet", propagated). Table 0x013CE558, 3.0f, 0.5f.
// Layout: as the landed CTCShotDefinitionDrainLife::UpdateCollision 0x007BDAC7 (PTargetThing +0x20, MaxCosAngleToTurn
//   +0x9c, MaxAngleToTurn +0xa0, Speed +0xa8); physics +0x60, its position +0x0c and velocity +0x28.
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

namespace NThingShot {

struct CTCShotDefinitionBase {
    void*                   __vftable;                      // +0x00
    CThing*                 ParentThing;                    // +0x04
    unsigned char           _pad08[0x20 - 0x08];
    CBaseIntelligentPointer PTargetThing;                   // +0x20
    unsigned char           _pad28[0x9c - 0x28];
};
struct CTCShotDefinitionDrainLife : CTCShotDefinitionBase {
    float                   MaxCosAngleToTurn;              // +0x9c
    float                   MaxAngleToTurn;                 // +0xa0
    float                   AmountToHeal;                   // +0xa4
    float                   Speed;                          // +0xa8
    CBaseIntelligentPointer PSoulEffect;                    // +0xac
    void UpdateMovement();
};

void CTCShotDefinitionDrainLife::UpdateMovement()
{
    CThing* target = PTargetThing.GetPItem();
    if (IS_THING_ALIVE(target)) {
        CTCPhysicsBase* physics = ParentThing->Physics;
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
        velocity *= Speed;
        physics->SetVelocity(velocity);
    }
}

}
