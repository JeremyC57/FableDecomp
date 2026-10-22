// CAIStateGroup_MeleeCombatNearbyBase::IsStillRunnable(bool) -- still runnable while the enemy lives, the creature's 0x30 component is engaged, the enemy can be engaged and is the creature's combat target, a perceived enemy is still seen/smelt or directly sensed, (when asked) the creature is a registered combatant, and the enemy is within the 0xAA component's range; once the enemy is running away its stealth component is flagged and the state stops
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093D800, length 637 bytes (+ int3 padding), `ret 4` (virtual).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound const @ 0x004365B0 (creature lookups 0x30,
// 8, 0x34, 0xAA) and @ 0x0040F020 (the enemy's 0x36 = CTCStealth), IsAbleToBeEngagedInCombat @ 0x0085FBC0 (__fastcall),
// the 8 component's virtual slot 29 (+0x74, combat target), CTCPerceiveThing::GetPerceivedThing @ 0x007446A0 /
// CanSeeOrSmellPerceivedThing @ 0x00661600 / GetDirectAwareness @ 0x00744430, IsRegisteredCombatant @ 0x00662410,
// CThing::GetPosition @ 0x004C73D0, IsEnemyRunningAway @ 0x0093D3A0, CTCStealth::SetAsRunningAwayFromCombat @ 0x007470E0.
// Layout: local model (PBrain +4; brain PCreature +0x18; PEnemy +0x14; presence bits +0x20, interface map +0x44, dead bit +0x91;
// 0x30 component engaged byte +0x80; 0xAA component range float +0x64).
// Shape: masked-test GetInterface lookups used unchecked (a miss reads an undefined slot); the early checks nest so they share one
// `return false` tail (early returns duplicate the epilogue and shrink-wrap esi); the combat target is a named local compared as
// `PEnemy == target`; one bool result kept in bl; vector subtract / squared length must be __forceinline.
struct CTCBase;
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
    __forceinline C3DVector operator-(const C3DVector& other) const { return C3DVector(X - other.X, Y - other.Y, Z - other.Z); }
    __forceinline float GetMagnitudeSquared() const { return X * X + Y * Y + Z * Z; }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
    const SEntry* LowerBound(const long* type) const;       // 0x004365B0
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1u << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1u << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CThingCreatureBase : CThing {
    bool IsRegisteredCombatant();               // 0x00662410
};
bool __fastcall IsAbleToBeEngagedInCombat(CThing* thing);  // 0x0085FBC0
struct CTCCombatEngage {                        // interface 0x30
    unsigned char _pad[0x80];
    bool Engaged;                               // +0x80
};
struct CTCCombatTarget {                        // interface 8
#define VF(n) virtual void v##n();
    VF(0) VF(1) VF(2) VF(3) VF(4) VF(5) VF(6) VF(7) VF(8) VF(9) VF(10) VF(11) VF(12) VF(13) VF(14) VF(15) VF(16) VF(17)
    VF(18) VF(19) VF(20) VF(21) VF(22) VF(23) VF(24) VF(25) VF(26) VF(27) VF(28)
    virtual CThing* GetTarget();                // slot 29 (+0x74)
};
struct CTCPerceiveThing {                       // interface 0x34
    CThing* GetPerceivedThing();                // 0x007446A0
    bool CanSeeOrSmellPerceivedThing();         // 0x00661600
    bool GetDirectAwareness();                  // 0x00744430
};
struct CTCCombatRange {                         // interface 0xAA
    unsigned char _pad[0x64];
    float Range;                                // +0x64
};
struct CTCStealth {                             // interface 0x36
    void SetAsRunningAwayFromCombat();          // 0x007470E0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
};
struct CAIStateGroup_MeleeCombatNearbyBase : CAIStateGroupBase {
    CBaseIntelligentPointer PEnemy;             // +0x14
    bool IsEnemyRunningAway();                  // 0x0093D3A0
    virtual bool IsStillRunnable(bool checkCombatant);
};

bool CAIStateGroup_MeleeCombatNearbyBase::IsStillRunnable(bool checkCombatant)
{
    if (IS_THING_ALIVE(PEnemy)) {
        const CTCCombatEngage* engage;
        ((const CThing*)GetCreature())->GetInterface(&engage, 0x30);
        if (engage->Engaged && IsAbleToBeEngagedInCombat(PEnemy.GetPItem())) {
            CTCCombatTarget* combat;
            ((const CThing*)GetCreature())->GetInterface(&combat, 8);
            CThing* target = combat->GetTarget();
            if (PEnemy.GetPItem() == target) {
                bool result = true;
                CTCPerceiveThing* perceive;
                if (((const CThing*)GetCreature())->GetInterface(&perceive, 0x34) && perceive->GetPerceivedThing() == PEnemy.GetPItem())
                    result = perceive->CanSeeOrSmellPerceivedThing() || perceive->GetDirectAwareness();
                if (result) {
                    if (checkCombatant)
                        result = GetCreature()->IsRegisteredCombatant();
                    if (result) {
                        const CThing* creature = GetCreature();
                        const CThing* enemy = PEnemy.GetPItem();
                        C3DVector offset = enemy->GetPosition() - creature->GetPosition();
                        const CTCCombatRange* range;
                        ((const CThing*)GetCreature())->GetInterface(&range, 0xaa);
                        float limit = range->Range;
                        if (offset.GetMagnitudeSquared() > limit * limit)
                            result = false;
                        else if (IsEnemyRunningAway()) {
                            CTCStealth* stealth;
                            if (PEnemy.GetPItem()->GetInterface(&stealth, 0x36))
                                stealth->SetAsRunningAwayFromCombat();
                            result = false;
                        }
                    }
                }
                return result;
            }
        }
    }
    return false;
}
