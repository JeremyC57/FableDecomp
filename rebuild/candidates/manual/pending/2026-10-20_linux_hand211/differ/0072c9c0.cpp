// CTCSpecialAbilities::SetupBattleCharge() -- unless the creature already has a battle-charge component, start a CCreatureAction_CastBattleCharge at its live target (or at nothing) and report whether the creature took the action
// Leaf: SetupBattleCharge
// Grade: DIFFER(211v224) -- everything after the first probes matches (tail-merged action arms); the lane folds the two bit tests of the same word into `test ch,3`, retail tests bit 9 then bit 8 through a 0x100 mask register it reuses for the targeting probe (bool HasTC, masked-word HasTC and split ifs all fold)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072C9C0, 224 bytes (plain `ret`).
// Callees: the const interface LowerBound @ 0x004365B0 (inlined GetTC of the targeting component, interface 8), its virtual slot
// 33 GetPTargetedThing (+0x84), CThing::GetPlayer() @ 0x004C7A10 (non-const; the player's byte at +0x20a),
// CCreatureAction_CastBattleCharge(CThingCreatureBase*, CThing*, bool, long) @ 0x008CE290 and (CThingCreatureBase*, long)
// @ 0x008CE2F0, CThingCreatureBase::SetCurrentAction(CCreatureActionBase*) @ 0x006644F0, ~CCombatActionBase @ 0x0062E900.
// Layout: as the landed SetupPhysicalShieldSpell 0x0072B0D0 (Creature at retail +0x18); CThing interface bitset +0x20
// (0xa8/0xa9 = bits 8/9 of the sixth word, 8 = bit 8 of the first), flag byte +0x6c (bit 3 targetable, bit 5 player-owned),
// dead flag bit 0 of +0x91; the action temporary is 0x130 bytes.
// Shape: each arm passes its own action temporary by address (sibling scopes share one frame slot, so VC merges the two
// identical SetCurrentAction/destructor tails); __declspec(novtable) action: the inline destructor only calls the combat
// base's.
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;  // 0x004365B0
};
struct CPlayer {
    unsigned char _pad00[0x20a];
    bool m_flag20a;                                    // +0x20a
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                    // +0x20
    CInterfaceMap m_interfaces;                        // +0x44
    unsigned char _pad4c[0x6c - 0x4c];
    unsigned char m_flags6c;                           // +0x6c
    unsigned char _pad6d[0x91 - 0x6d];
    unsigned char m_flags91;                           // +0x91 bit 0: dead
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
    CPlayer* GetPlayer();                              // 0x004C7A10 (non-const; 0x004C7A30 is the const twin)
};
struct CTCTargetingBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32();
    virtual CThing* GetPTargetedThing();               // slot 33 (+0x84)
};
class CThingCreatureBase;
struct CCreatureActionBase {
    unsigned char _body[0xa8];
};
struct CCombatActionBase : CCreatureActionBase {
    unsigned char _combat[0x114 - 0xa8];
    ~CCombatActionBase();                              // 0x0062E900
};
struct __declspec(novtable) CCreatureAction_CastBattleCharge : CCombatActionBase {
    unsigned char _own[0x130 - 0x114];
    CCreatureAction_CastBattleCharge(CThingCreatureBase* creature, CThing* target, bool aggressive, long flags);   // 0x008CE290
    CCreatureAction_CastBattleCharge(CThingCreatureBase* creature, long flags);                                   // 0x008CE2F0
};
class CThingCreatureBase : public CThing {
public:
    bool SetCurrentAction(CCreatureActionBase* action); // 0x006644F0
};
struct CTCSpecialAbilities {
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _base08[4];
    bool AbilityMutexLock;                      // +0x0c
    bool UsingBed;                              // +0x0d
    long CurrentAbility;                        // +0x10
    void* UndeadFollowers;                      // +0x14 list sentinel
    CThingCreatureBase* Creature;               // +0x18 (PDB +0x20)
    __forceinline CThing* GetChargeTarget()
    {
        if (Creature->HasTC(8)) {
            CThing* target = ((CTCTargetingBase*)Creature->GetTC(8))->GetPTargetedThing();
            if (target && !(target->m_flags91 & 1) && (target->m_flags6c & 8))
                return target;
        }
        return 0;
    }
    __forceinline bool IsPlayerAggressive()
    {
        bool aggressive = false;
        if (Creature->m_flags6c & 0x20)
            aggressive = Creature->GetPlayer()->m_flag20a;
        return aggressive;
    }
    bool SetupBattleCharge();
};

bool CTCSpecialAbilities::SetupBattleCharge()
{
    if (Creature->HasTC(0xa9) || Creature->HasTC(0xa8))
        return false;
    CThing* target = GetChargeTarget();
    if (target)
        return Creature->SetCurrentAction(&CCreatureAction_CastBattleCharge(Creature, target, IsPlayerAggressive(), 0));
    return Creature->SetCurrentAction(&CCreatureAction_CastBattleCharge(Creature, 0));
}
