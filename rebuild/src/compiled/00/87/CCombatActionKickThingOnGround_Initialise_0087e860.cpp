// CCombatAction_KickThingOnGround::Initialise() -- play the stealth kick on a living type-13 target (aim 1.0 forward / 0.15 aside) or the hard low kick (0.0 / 0.1); against a living target suspend its animation (type 1), make it and the kicker not solid to each other and aim at its closest hit-location point (else its position); MoveToPos keeps the kicker's height and is pushed along and beside the kicker's back-facing direction, then the base Initialise runs
// Leaf: ?Initialise@CCombatAction_KickThingOnGround@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: map pointer made const (the row grades the const LowerBound) re-padded by-value maps +8.
// Naming wave 18: the shared const float 1e-4f at 0x0129BA3C is ?DELTA_NOTIONAL_ZERO@@3MB (raw Ego_r public + typed data record; was EPSILON).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0087E860, 880 bytes, plain `ret` (virtual, vtable 0x01277D3C).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PCreature +8, PTarget +0xa8), CCharString ctor/dtor 0x0099EBF0 /
//   0x0099EAE0, CCombatActionBase::SetCombatAnimation 0x008574D0 and Initialise 0x00857520, CThing::GetPos 0x004C73D0,
//   the physics virtual slot 70 GetFacing (+0x118), CThingAICreature::SuspendAI 0x00833A10, CThing::AddTC
//   0x004C9D60 ("CTCSolidToThing"), CTCSolidToThing::AddThingNotSolidTo 0x00770FA0, the interface-map LowerBound
//   0x004365B0 (interface 0x7F), its component's hit-location getter 0x00814BF0 (`mov eax,[ecx+18h]`),
//   CHitLocation::GetClosestPointToPos2D 0x00815370 (hidden C3DVector result, position and direction by reference),
//   C3DVector::AccurateNormalise @ 0x00A14440 (fsqrt form). Data: epsilon 1e-4f 0x0129BA3C (read as a variable, squared at run time).
// Layout: retail CCombatActionBase is 0x114 bytes (PTarget +0xa8); Ego_r.pdb KickThingOnGround MoveToPos / Movement /
//   MaxRunningSpeed at PDB +0x12c/+0x138/+0x144 sit 0x18 lower in retail (+0x114/+0x120/+0x12c, cf. the landed ctor
//   0x0087FD20). CThing: ThingType byte +0x10, interface bits +0x20, map +0x44, physics +0x60, dead flag +0x91 bit 0.
// Shape: the animation names are block-scoped so each arm destroys its string before the two aim factors are stored; the
//   facing is a named local negated through the inline operator- (frame 0x58); C3DVector has a user copy ctor, so the first
//   `position` copy drops its dead Z word; the suspend target is a named local (GetPItem before the argument pushes); the
//   2D length test is `X*X + Y*Y > eps*eps` after `direction.Z = 0`; the aim is `direction * forward + position +
//   C3DVector(direction.Y, -direction.X, 0.0f) * side` (the perpendicular term first, then the forward sum).
#include "fable_compiler.h"
extern const float DELTA_NOTIONAL_ZERO;     // 0x0129BA3C (1e-4f)

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator-() const { return C3DVector(-X, -Y, -Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector& AccurateNormalise();                                   // 0x00A14440 (Ego_r ?AccurateNormalise@C3DVector@@QAEAAV1@XZ)
};

#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
class CTCBase;

#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
struct CHitLocation {
    C3DVector GetClosestPointToPos2D(const C3DVector& position, const C3DVector& direction);   // 0x00815370
};
class CTCHitLocations { public:                                // interface 0x7F
    CHitLocation* GetHitLocation() const;                     // 0x00814BF0
};
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
class CTCPhysicsBase { public:
    VF10(0) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF10(6)
    virtual C3DVector GetFacing();                      // slot 70 (+0x118)
};
class CThing;
class CTCSolidToThing { public:
    void AddThingNotSolidTo(CThing* thing);             // 0x00770FA0
};
class CTCParamsBase;

class CThingAICreature : public CThing {
public:
    void SuspendAI(bool suspend, void* suspender);   // 0x00833A10 (Ego_r.pdb CThingAICreature::SuspendAI)
public: public:
};
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

FABLE_FORCEINLINE bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    return p.Get() && !p.Get()->IsDead();
}

class CCombatActionBase {
public:
    virtual void Initialise();                          // 0x00857520
    unsigned char           _pad04[4];
    CIntelligentPointer<CThing> PCreature;                  // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    CIntelligentPointer<CThing> PTarget;                    // +0xa8
    unsigned char           _padb0[0x114 - 0xb0];
    void SetCombatAnimation(const CCharString& name);     // 0x008574D0
};
class CCombatAction_KickThingOnGround : public CCombatActionBase {
public:
    C3DVector MoveToPos;                                // +0x114
    C3DVector Movement;                                 // +0x120
    float     MaxRunningSpeed;                          // +0x12c
    virtual void Initialise();
};

void CCombatAction_KickThingOnGround::Initialise()
{
    float forward;
    float side;
    if (IS_THING_ALIVE(PTarget) && PTarget.Get()->GetType() == 13) {
        {
            CCharString name("STRIKE_STEALTH_KICK_THING_ON_GROUND");
            SetCombatAnimation(name);
        }
        forward = 1.0f;
        side = 0.15f;
    } else {
        {
            CCharString name("STRIKE_HARD_LOW_KICK");
            SetCombatAnimation(name);
        }
        forward = 0.0f;
        side = 0.1f;
    }
    CTCPhysicsBase* physics = PCreature.Get()->GetPhysicsTC();
    C3DVector position = PCreature.Get()->GetPos();
    C3DVector facing = physics->GetFacing();
    C3DVector direction = -facing;
    if (IS_THING_ALIVE(PTarget)) {
        if (PTarget.Get()->GetType() == 1) {
            CThingAICreature* creature = (CThingAICreature*)PTarget.Get();
            ((CThingAICreature*)creature)->SuspendAI(true, 0);
        }
        CTCSolidToThing* targetSolid;
        {
            CCharString name("CTCSolidToThing");
            targetSolid = ((CTCSolidToThing*)PTarget.Get()->AddTC(name, true, 0));
        }
        CTCSolidToThing* creatureSolid;
        {
            CCharString name("CTCSolidToThing");
            creatureSolid = ((CTCSolidToThing*)PCreature.Get()->AddTC(name, true, 0));
        }
        targetSolid->AddThingNotSolidTo(PCreature.Get());
        creatureSolid->AddThingNotSolidTo(PTarget.Get());
        position = PTarget.Get()->GetPos();
        const CTCHitLocations* hitLocations;
        if (PTarget.Get()->PeekTC(TCI_HIT_LOCATIONS, &hitLocations)) {
            CHitLocation* location = hitLocations->GetHitLocation();
            if (location)
                position = location->GetClosestPointToPos2D(PCreature.Get()->GetPos(), direction);
        }
    }
    position.Z = PCreature.Get()->GetPos().Z;
    MoveToPos = position;
    direction.Z = 0.0f;
    if (direction.X * direction.X + direction.Y * direction.Y > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO) {
        direction.AccurateNormalise();
        MoveToPos = direction * forward + position + C3DVector(direction.Y, -direction.X, 0.0f) * side;
    }
    CCombatActionBase::Initialise();
}