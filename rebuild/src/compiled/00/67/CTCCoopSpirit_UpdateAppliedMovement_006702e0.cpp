// CTCCoopSpirit::UpdateAppliedMovement() -- unless the spirit's creature is busy with an action, take (and clear) the accumulated movement vector, turn it by the creature's relative movement angle, normalise it and, when it is not degenerate, reset the no-movement timer and push the creature's physics along it at half its length
// Leaf: UpdateAppliedMovement
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006702E0, 400 bytes (`ret`).
// Callees: CThingCreatureBase::IsPerformingAnyAction() @ 0x006622A0, CThingPlayerCreature::GetCurrentRelativeMovementAngleXY()
// @ 0x006A9640, C3DVector::RotateAboutZ(float) @ 0x00A14040, C3DVector::Normalise() @ 0x00A14440, CVectorMap<ETCInterfaceType,
// CTCBase*>::LowerBound @ 0x0040F020 (interface 2, the physics component), CTCPhysicsBase::ApplyAcceleration @ 0x006B0210.
// Globals: 1e-4f @ 0x0129BA3C, the reciprocal-square-root mantissa table 0x013CE558, 3.0f @ 0x0122DED4, 0.5f @ 0x0122F59C.
// Layout: as the landed UpdateAttractionToMaster 0x006701A0 (NoMovementTimer +0x0c, MovementVector +0x18); the physics
// component pointer is deliberately unset when absent (retail reads a dead slot).
// Shape: the movement vector is copied through a user copy constructor and cleared with an inline Zero() (`X = Y = Z = 0`,
// stored Z, Y, X; the zero is kept in ebx for the timer too); the fast Normalise sums `(x * x + y * y) + z * z` with explicit
// parentheses (the unparenthesised sum is CSE'd whole with GetLength's; retail shares only the three squares), otherwise the
// landed SetupFacingDirection 0x007A6E20 fast Normalise; the scale is `*= length * 0.5f`.
#include <math.h>
#pragma intrinsic(sqrt)
extern unsigned long g_InvSqrtMantissaTable[128];          // 0x013CE558
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    void Zero() { X = Y = Z = 0.0f; }
    float GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
    __forceinline void FastNormalise()
    {
        float x = X;
        float y = Y;
        float z = Z;
        float lengthSquared = (x * x + y * y) + z * z;
        unsigned long bits = *(unsigned long*)&lengthSquared;
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = (3.0f - r * r * lengthSquared) * r * 0.5f;
        X *= r;
        Y *= r;
        Z *= r;
    }
    C3DVector& operator*=(float f) { X *= f; Y *= f; Z *= f; return *this; }
    void RotateAboutZ(float angle);                         // 0x00A14040
    void Normalise();                                       // 0x00A14440
};
struct CTCBase;
struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
    __forceinline CTCBase* GetTCIfPresent(long interfaceType)
    {
        CTCBase* tc;                                        // deliberately unset when absent (retail reads a dead slot)
        if (HasTC(interfaceType))
            tc = GetTC(interfaceType);
        return tc;
    }
};
struct CThingPlayerCreature : CThing {
    bool IsPerformingAnyAction();                           // 0x006622A0
    float GetCurrentRelativeMovementAngleXY() const;        // 0x006A9640
};
struct CTCPhysicsBase {
    void ApplyAcceleration(const C3DVector& acceleration);  // 0x006B0210
};
struct CTCCoopSpirit {
    virtual void v00();
    CThingPlayerCreature* ParentThing;                      // +0x04 (CTCBase)
    unsigned char _pad08[0x0c - 0x08];
    long    NoMovementTimer;                                // +0x0c
    long    Score;                                          // +0x10
    long    OffscreenCounter;                               // +0x14
    C3DVector MovementVector;                               // +0x18
    void UpdateAppliedMovement();
};

void CTCCoopSpirit::UpdateAppliedMovement()
{
    CThingPlayerCreature* creature = ParentThing;
    if (creature->IsPerformingAnyAction())
        return;
    C3DVector movement = MovementVector;
    MovementVector.Zero();
    float angle = creature->GetCurrentRelativeMovementAngleXY();
    movement.RotateAboutZ(angle);
    movement.Normalise();
    CTCPhysicsBase* physics = (CTCPhysicsBase*)ParentThing->GetTCIfPresent(2);
    float length = movement.GetLength();
    if (length > 1e-4f) {
        NoMovementTimer = 0;
        movement.FastNormalise();
        movement *= length * 0.5f;
        physics->ApplyAcceleration(movement);
    }
}