// CCreatureAction_ScorpionCharge::FinishAction() -- base FinishAction; let a live target's 0x93 shield react to the
// scorpion again; then, if the target still lives, either keep charging the remaining distance or (out of distance /
// barely moved) drop component 0xC8 and strike
// Leaf: ?FinishAction@CCreatureAction_ScorpionCharge
// Layout: retail CCombatActionBase is 0x114 (Ego_r.pdb 0x12c): ChargeDistance +0x114, StartPos +0x118 (PDB +0x12c/+0x130);
//   PTarget +0xa8 (PDB +0xb4); CThing interface bitset +0x20, map +0x44, dead flag +0x91 bit 0 (landed 0x008C5490).
// Shape: travelled distance is the XY length from StartPos (inline fsqrt); a discarded (compiled-out) debug call with
//   (target position, creature position) keeps both GetPosition calls -- the target through an inline accessor, so its
//   GetPItem runs first; IS_THING_ALIVE-style inline twice (bl = 1 reused for the dead-bit test); both temporaries are
//   destroyed through ~CCombatActionBase on one merged tail.
#include <math.h>
struct C3DVector { float X, Y, Z; };
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);             // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                   // +0x20
    CInterfaceMap m_interfaces;                       // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                          // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;             // 0x004C73D0
    void RemoveTC(long type);                         // 0x004C9840
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CCreatureActionBase;
struct CThingCreatureBase : CThing {
    bool IsCollidingWithLandscape() const;            // 0x00661E70 (manifest: CanCollideWithLandscape)
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};
struct CTCPhysicalShieldSpell {
    void AllowReactionsOnHitFromAttacker(CThing* attacker, bool allow);   // 0x0079E580
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                         // 0x00A01B50
};
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;                // +0x08
    CThingCreatureBase* GetCreature() const { return (CThingCreatureBase*)PCreature.GetPItem(); }
};
struct CCombatActionBase : CCreatureActionBase {
    unsigned char _pad10[0xa8 - 0x10];
    CBaseIntelligentPointer PTarget;                  // +0xa8
    unsigned char _padb0[0x114 - 0xb0];
    ~CCombatActionBase();                             // 0x0062E900
    void FinishAction();                              // 0x00859A20
    CThing* GetTarget() const { return PTarget.GetPItem(); }
    bool IsTargetAlive() const { return PTarget.GetPItem() && !(PTarget.GetPItem()->m_flags91 & 1); }
};
struct CCreatureAction_ScorpionCharge : CCombatActionBase {
    float     ChargeDistance;                         // +0x114
    C3DVector StartPos;                               // +0x118
    CCreatureAction_ScorpionCharge(CThingCreatureBase& owner, CThing& target, float distance);   // 0x008C4EA0
    void FinishAction();
};
struct CCreatureAction_ScorpionChargeStrike : CCombatActionBase {
    unsigned char _pad114[0x10];
    CCreatureAction_ScorpionChargeStrike(CThingCreatureBase* owner, CThing* target);   // 0x008C4F50
};
inline void DebugLine(const C3DVector& from, const C3DVector& to) { }

void CCreatureAction_ScorpionCharge::FinishAction()
{
    const C3DVector& pos = GetCreature()->GetPosition();
    float dx = pos.X - StartPos.X;
    float dy = pos.Y - StartPos.Y;
    float travelled = (float)sqrt(dx * dx + dy * dy);
    float remaining = ChargeDistance - travelled;
    DebugLine(GetTarget()->GetPosition(), GetCreature()->GetPosition());
    CCombatActionBase::FinishAction();
    if (IsTargetAlive()) {
        CTCPhysicalShieldSpell* shield;
        if (PTarget.GetPItem()->GetInterface(&shield, 0x93))
            shield->AllowReactionsOnHitFromAttacker(GetCreature(), false);
    }
    if (IsTargetAlive()) {
        if (remaining > 0.0f && travelled > 0.1f) {
            if (!GetCreature()->IsCollidingWithLandscape())
                GetCreature()->SetCurrentAction(CCreatureAction_ScorpionCharge(*GetCreature(), *PTarget.GetPItem(), remaining));
        } else {
            GetCreature()->RemoveTC(0xc8);
            if (!GetCreature()->IsCollidingWithLandscape())
                GetCreature()->SetCurrentAction(CCreatureAction_ScorpionChargeStrike(GetCreature(), PTarget.GetPItem()));
        }
    }
}