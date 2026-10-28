// 008f4d60 CCreatureAction_DragonFlight::CCreatureAction_DragonFlight
// Flags: /O2 /Oy /W3
// Leaf: ??0CCreatureAction_DragonFlight
// The dragon-flight action towards a point: base CCombatActionBase(owner, 50, zeroed CSoundPair), FaceTarget off,
// own vtable, the animation name handed to the base setter. The turn it needs is the bearing to the point minus
// the owner's facing bearing, both in turns (atan2 / 2pi wrapped into [0, 1)), the difference wrapped into
// [-0.5, 0.5).
// Callees: CCombatActionBase::CCombatActionBase(owner, long, const CSoundPair&) 0x00858030,
// CCombatActionBase::SetCombatAnimation 0x008574D0, the physics TC's slot 70 GetFacing (+0x118, hidden result),
// CThing::GetPos 0x004C73D0, CRT fmod 0x00BFEB28.
// Vtable 0x01288544. Layout: retail CCombatActionBase is 0x114 bytes (PDB 0x12c), so FaceTarget +0x114 and
// TurnRate +0x118 (PDB +0x12c/+0x130), as in the landed sibling ctor 0x008F4D00; CThing PhysicsTC +0x60.
#include <math.h>
#include "fable_string.h"
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
class CTCPhysicsBase { public:                                 // CThing::PhysicsTC (+0x60)
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF10(6)
#undef VF10
#undef VF
    virtual C3DVector GetFacing() const;                // slot 70 (+0x118)
};
class CThing { public:
    unsigned char   _pad0[0x60];
    CTCPhysicsBase* PhysicsTC;                          // +0x60
    const C3DVector& GetPos() const;               // 0x004C73D0
    CTCPhysicsBase* GetPhysics() const { return PhysicsTC; }
};
class CThingCreatureBase : public CThing { public: };
class CSoundPair { public:
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
class CCombatActionBase { public:                              // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
public:
    void SetCombatAnimation(const CCharString&);   // 0x008574D0
};
// Bearing of a direction in turns, wrapped into [0, 1).
static __forceinline float GetBearingInTurns(float x, float y)
{
    float turns = (float)atan2((double)x, (double)y) * 0.15915494f;
    if (turns < 0.0f || turns >= 1.0f) {
        turns = (float)fmod((double)turns, 1.0);
        if (turns < 0.0f)
            turns += 1.0f;
    }
    return turns;
}
// Wrap a turn difference into [-0.5, 0.5).
static __forceinline float WrapTurnDifference(float turns)
{
    if (turns < -0.5f)
        return (float)fmod((double)(turns - 0.5f), 1.0) + 0.5f;
    if (turns >= 0.5f)
        return (float)fmod((double)(turns - -0.5f), 1.0) - 0.5f;
    return turns;
}
class CCreatureAction_DragonFlight : public CCombatActionBase { public:   // vtable 0x01288544
    bool  FaceTarget;                                   // +0x114
    float TurnRate;                                     // +0x118
    virtual void v00();
    CCreatureAction_DragonFlight(CThingCreatureBase* owner, const CCharString& animName, const C3DVector& target);
};
CCreatureAction_DragonFlight::CCreatureAction_DragonFlight(CThingCreatureBase* owner, const CCharString& animName,
                                                           const C3DVector& target)
    : CCombatActionBase(owner, 50, CSoundPair())
    , FaceTarget(false)
{
    SetCombatAnimation(animName);
    C3DVector facing = owner->GetPhysics()->GetFacing();
    const C3DVector& position = owner->GetPos();
    float targetBearing = GetBearingInTurns(target.X - position.X, target.Y - position.Y);
    float facingBearing = GetBearingInTurns(facing.X, facing.Y);
    TurnRate = WrapTurnDifference(targetBearing - facingBearing);
}