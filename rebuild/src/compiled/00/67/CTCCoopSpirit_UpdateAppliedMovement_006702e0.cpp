// CTCCoopSpirit::UpdateAppliedMovement() -- unless the spirit's creature is busy with an action, take (and clear) the accumulated movement vector, turn it by the creature's relative movement angle, normalise it and, when it is not degenerate, reset the no-movement timer and push the creature's physics along it at half its length
// Leaf: UpdateAppliedMovement
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Naming wave 18: g_InvSqrtMantissaTable is GFastOneOverSqrtTable (?GFastOneOverSqrtTable@@3PAKA); ChatGPT pass 10 audit, raw Ego_r public.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006702E0, 400 bytes (`ret`).
// Callees: CThingCreatureBase::IsPerformingAnyAction() @ 0x006622A0, CThingPlayerCreature::GetCurrentRelativeMovementAngleXY()
// @ 0x006A9640, C3DVector::RotateXY(float) @ 0x00A14040, C3DVector::AccurateNormalise @ 0x00A14440, CVectorMap<ETCInterfaceType,
// CTCBase*>::LowerBound @ 0x0040F020 (interface 2, the physics component), CTCPhysicsBase::ApplyAcceleration @ 0x006B0210.
// Globals: 1e-4f @ 0x0129BA3C, the reciprocal-square-root mantissa table 0x013CE558, 3.0f @ 0x0122DED4, 0.5f @ 0x0122F59C.
// Layout: as the landed UpdateAttractionToMaster 0x006701A0 (NoMovementTimer +0x0c, MovementVector +0x18); the physics
// component pointer is deliberately unset when absent (retail reads a dead slot).
// Shape: the movement vector is copied through a user copy constructor and cleared with an inline Zero() (`X = Y = Z = 0`,
// stored Z, Y, X; the zero is kept in ebx for the timer too); the fast Normalise sums `(x * x + y * y) + z * z` with explicit
// parentheses (the unparenthesised sum is CSE'd whole with GetLength's; retail shares only the three squares), otherwise the
// landed SetupFacingDirection 0x007A6E20 fast Normalise; the scale is `*= length * 0.5f`.
#include "fable_compiler.h"
#include <math.h>
#pragma intrinsic(sqrt)
extern unsigned long GFastOneOverSqrtTable[128];          // 0x013CE558
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    void Zero() { X = Y = Z = 0.0f; }
    float GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
    FABLE_FORCEINLINE void FastNormalise()
    {
        float x = X;
        float y = Y;
        float z = Z;
        float lengthSquared = (x * x + y * y) + z * z;
        unsigned long bits = *(unsigned long*)&lengthSquared;
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (GFastOneOverSqrtTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = (3.0f - r * r * lengthSquared) * r * 0.5f;
        X *= r;
        Y *= r;
        Z *= r;
    }
    C3DVector& operator*=(float f) { X *= f; Y *= f; Z *= f; return *this; }
    C3DVector& AccurateNormalise();                                       // 0x00A14440 (Ego_r ?AccurateNormalise@C3DVector@@QAEAAV1@XZ)
public:
    void RotateXY(float);   // 0x00A14040
};
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
    FABLE_FORCEINLINE unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    FABLE_FORCEINLINE CTCBase* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound(type);
        return (it == map->end() || interfaceType < it->first ? map->end() : it)->second;
    }
    FABLE_FORCEINLINE CTCBase* GetTCIfPresent(long interfaceType)
    {
        CTCBase* tc;                                        // deliberately unset when absent (retail reads a dead slot)
        if (HasTC(interfaceType))
            tc = GetTC(interfaceType);
        return tc;
    }
};
// Name fix (naming agent NF21, 2026-11-11): IsPerformingAnyAction is the inherited const CThingCreatureBase member
//   ?IsPerformingAnyAction@CThingCreatureBase@@QBE_NXZ (Ego_r.pdb / FableWin.pdb; RTTI base CThingCreatureBase).
class CThingCreatureBase : public CThing { public:
    bool IsPerformingAnyAction() const;                     // 0x006622A0
};
class CThingPlayerCreature : public CThingCreatureBase { public:
    float GetCurrentRelativeMovementAngleXY() const;        // 0x006A9640
};
class CTCPhysicsBase { public:
    void ApplyAcceleration(const C3DVector& acceleration);  // 0x006B0210
};
class CTCCoopSpirit { public:
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
    movement.RotateXY(angle);
    movement.AccurateNormalise();
    CTCPhysicsBase* physics = (CTCPhysicsBase*)ParentThing->GetTCIfPresent(2);
    float length = movement.GetLength();
    if (length > 1e-4f) {
        NoMovementTimer = 0;
        movement.FastNormalise();
        movement *= length * 0.5f;
        physics->ApplyAcceleration(movement);
    }
}