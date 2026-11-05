// CCreatureAction_SummonerLightningOrbInto::FrameUpdate() -- base combat FrameUpdate; while the target lives, turn the summoner's facing towards it in the ground plane by at most 1/36 of a revolution per frame (clamped dot, sign from the cross product) and set the summoner's orientation to (up, turned facing)
// Leaf: ?FrameUpdate@CCreatureAction_SummonerLightningOrbInto@@UAEXXZ
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CAD50, 441 bytes, plain `ret` (virtual; Ego_r.pdb UAEXXZ).
// Callees: CCombatActionBase::FrameUpdate() 0x00859D80 (qualified call of the base virtual), CBaseIntelligentPointer::GetPItem
//   0x00A01B50, CThing::GetPos() const 0x004C73D0, C3DVector::AccurateNormalise() 0x00A14440 (returns *this; sqrt
//   normalise), the physics TC's virtuals GetFacingVector() const (slot 70, +0x118; the base body returns (0, 1, 0)) and
//   SetRHSet(const CRightHandedSet&) (slot 75, +0x12C; chunk 608 proof), acos (_CIacos 0x00BFEBA2), C3DVector::Rotate(const
//   C3DVector&, float) 0x00A13C60.
// Layout: retail CCreatureActionBase 0xa8 (PCreature +0x08, PAnimEntry +0x74; engine/CCreatureActionBase.h), retail
//   CCombatActionBase PTarget +0xA8 (Ego_r +0xB4), CThing PhysicsTC +0x60 (Ego_r +0x70), Dead bit 0 of +0x91. The generated
//   engine/CCreatureAction_SummonerLightningOrbInto.h is all padding with the vptr as a data member, so it cannot host the
//   virtual FrameUpdate chain (base qualified call + own UAE symbol); the classes are modelled locally with real virtuals.
// Shape: the dead test through CThing::IsDead() const (fixes the this/&PTarget esi/edi colouring); the clamped dot as ONE
//   expression in the order direction.Z*facing.Z + direction.X*facing.X + direction.Y*facing.Y (VC7.1 evaluates the sum from
//   the right, giving retail's Y, X, Z products, and the same form fixes the cross product's x87 operand order, which
//   follows the dot's value numbering: the compound `dot = ...; dot += ...;` form commutes the cross's fmul operands);
//   the angle clamp as `angle = max; if (max > turn) angle = turn;`; the physics TC through the PDB's inline
//   CThing::GetPhysicsTC() (the bare member load colours the TC pointer eax where retail has ecx).
#include <math.h>
#include "engine/C3DVector.h"
inline C3DVector CrossProduct(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X);
}
class CRightHandedSet {
public:
    C3DVector Up;
    C3DVector Forward;
    // Unproven inline (not in the Ego_r/FableWin name sets, never emitted): the PDB two-argument (up, forward) constructor
    // spelled at the call grows the frame to 0x44 (465v441), so the up vector is built inside a one-argument form.
    CRightHandedSet(const C3DVector& forward) : Up(0.0f, 0.0f, 1.0f), Forward(forward) {}
};
// Slots 0..69 and 71..74 of CTCPhysicsBase are not used here and not named (filler declarations keep the two used slots
// at their retail offsets; they are never called or emitted).
#define UNPROVEN_SLOT(n) virtual void UnprovenSlot##n();
#define UNPROVEN_SLOTS10(p) UNPROVEN_SLOT(p##0) UNPROVEN_SLOT(p##1) UNPROVEN_SLOT(p##2) UNPROVEN_SLOT(p##3) UNPROVEN_SLOT(p##4) \
    UNPROVEN_SLOT(p##5) UNPROVEN_SLOT(p##6) UNPROVEN_SLOT(p##7) UNPROVEN_SLOT(p##8) UNPROVEN_SLOT(p##9)
class CTCPhysicsBase {
public:
    UNPROVEN_SLOTS10(0) UNPROVEN_SLOTS10(1) UNPROVEN_SLOTS10(2) UNPROVEN_SLOTS10(3) UNPROVEN_SLOTS10(4) UNPROVEN_SLOTS10(5)
    UNPROVEN_SLOTS10(6)
    virtual C3DVector GetFacingVector() const;              // slot 70 (+0x118)
    UNPROVEN_SLOT(71) UNPROVEN_SLOT(72) UNPROVEN_SLOT(73) UNPROVEN_SLOT(74)
    virtual void SetRHSet(const CRightHandedSet& set);      // slot 75 (+0x12C)
};
#include "engine/CThing.h"
class CThingCreatureBase : public CThing { };
class CAnimationEntry;
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CCreatureActionBase {                           // retail sizeof 0xa8
public:
    virtual ~CCreatureActionBase();
    unsigned char _pad04[4];
    CIntelligentPointer<CThingCreatureBase> PCreature;   // +0x08
    unsigned char _pad10[0x74 - 0x10];
    CAnimationEntry* PAnimEntry_Data;                 // +0x74
    unsigned char _pad78[0xa8 - 0x78];
};
class CCombatActionBase : public CCreatureActionBase {   // retail sizeof 0x114
public:
    CIntelligentPointer<CThing> PTarget;              // +0xA8
    unsigned char _padb0[0x114 - 0xb0];
    virtual void FrameUpdate();                       // 0x00859D80
};
class CCreatureAction_SummonerLightningOrbInto : public CCombatActionBase {
public:
    virtual void FrameUpdate();
};

void CCreatureAction_SummonerLightningOrbInto::FrameUpdate()
{
    CCombatActionBase::FrameUpdate();
    if (PTarget.Get() && !PTarget.Get()->IsDead()) {
        CThing* target = PTarget.Get();
        const C3DVector& from = PCreature.Get()->GetPos();
        const C3DVector& to = target->GetPos();
        C3DVector direction(to.X - from.X, to.Y - from.Y, 0.0f);
        direction.AccurateNormalise();
        C3DVector facing = PCreature.Get()->GetPhysicsTC()->GetFacingVector();
        float dot = direction.Z * facing.Z + direction.X * facing.X + direction.Y * facing.Y;
        if (dot < -1.0f)
            dot = -1.0f;
        else if (dot > 1.0f)
            dot = 1.0f;
        float angle = 0.027777778f;
        float turn = fabs(acos(dot) * 0.15915494f);
        if (0.027777778f > turn)
            angle = turn;
        if (CrossProduct(facing, direction).Z > 0.0f)
            angle *= -1.0f;
        facing.Rotate(C3DVector(0.0f, 0.0f, 1.0f), angle);
        PCreature.Get()->GetPhysicsTC()->SetRHSet(CRightHandedSet(facing));
    }
}