// CDefendingCombatantInfo::CCombatRing::StretchRingThickness(const CThingCreatureBase& defender, const CThingCreatureBase& attacker) -- widen the ring to fit the attacker: an outer ring to 1.5 times the attacker's diameter, the innermost ring to the attacker's combat-type front-far limit, never narrowing it
// Leaf: ?StretchRingThickness@CCombatRing@CDefendingCombatantInfo@@QAEXABVCThingCreatureBase@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E8050, 73 bytes, thiscall, `ret 8`.
// Identity (proven): Ego_r.pdb ?StretchRingThickness@CCombatRing@CDefendingCombatantInfo@@QAEXABVCThingCreatureBase@@0@Z
//   (two creature references, `ret 8`); the body reads only Ego_r CCombatRing members (Index +0x08, RingThickness +0x0C)
//   and sits with the other CCombatRing / CCombatManager code (GetAllProngs 0x006EAB00). No direct callers (inlined
//   elsewhere); the catalog label already names it.
// Callees: CThingCreatureBase::GetCombatTypeDef() const 0x00666D10 (Ego_r.pdb, returns const CCombatTypeDef&).
//   Float pool 0x0125AADC = 1.5f.
// Layout: the attacker's physics body +0x60, its radius +0x34 (the landed navigator models); CCombatTypeDef +0x6C,
//   FrontFarLimit by the Ego_r order with the retail 12-byte vectors (probable member name).
// Shape: each branch computes the wanted thickness into a float and stores it only when it is larger; the outer-ring
//   multiply happens after the compare (on the store path).
class CPhysicsBody { public:
    unsigned char _pad00[0x34];
    float         Radius;                           // +0x34
};
class CCombatTypeDef { public:
    unsigned char _pad00[0x6C];
    float         FrontFarLimit;                    // +0x6C (retail)
};
class CThingCreatureBase { public:
    unsigned char _pad00[0x60];
    CPhysicsBody* PBody;                            // +0x60
    const CCombatTypeDef& GetCombatTypeDef() const; // 0x00666D10
};
class CDefendingCombatantInfo {
public:
    class CCombatRing {
    public:
        float RotationAngle;                        // +0x00
        float DeltaRotation;                        // +0x04
        long  Index;                                // +0x08
        float RingThickness;                        // +0x0C
        void StretchRingThickness(const CThingCreatureBase& defender, const CThingCreatureBase& attacker);
    };
};

void CDefendingCombatantInfo::CCombatRing::StretchRingThickness(const CThingCreatureBase& defender, const CThingCreatureBase& attacker)
{
    if (Index != 0) {
        float diameter = attacker.PBody->Radius + attacker.PBody->Radius;
        if (diameter > RingThickness)
            RingThickness = diameter * 1.5f;
    }
    else {
        float limit = attacker.GetCombatTypeDef().FrontFarLimit;
        if (limit > RingThickness)
            RingThickness = limit;
    }
}