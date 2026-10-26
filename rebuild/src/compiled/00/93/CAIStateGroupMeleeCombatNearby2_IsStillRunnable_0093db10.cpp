// CAIStateGroup_MeleeCombatNearby2::IsStillRunnable(bool) -- still runnable when the base says so AND the enemy
// and the creature stand on the same navigation region (the region lookup takes each thing's position).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093DB10, length 94 bytes (+ int3 padding), `ret 4`.
// Callees: CAIStateGroup_MeleeCombatNearbyBase::IsStillRunnable(bool) @ 0x0093D800 (the argument forwarded),
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PEnemy at +0x14, twice), CThing::GetPosition @ 0x004C73D0
// (const C3DVector&), the creature's region-of-position query @ 0x00661D20 (manifest "CFrontEndManager::
// CreateComponent" is a propagated label: it takes a C3DVector* and returns a long, called on both things).
// Shape: `if (Base(arg)) { enemy = GetPItem(); creature = PBrain->Creature; a = enemy->Region(GetPItem()->
// GetPosition()); b = creature->Region(creature->GetPosition()); if (a == b) return true; } return false;` --
// the nested-if form gives the single shared `xor al,al` tail that both misses jump to (an early `return
// false` guard emits its own epilogue, and `if (a != b) return false; return true;` becomes a `sete`); the
// named enemy/creature locals land in ebx/edi, the first region in esi. Member names from the engine header (PEnemy +0x14, Archer +0x1c); the header
// flattens the base, so the layout is local.
class C3DVector;

class CThing { public:
    const C3DVector& GetPosition();         // 0x004C73D0
    long GetRegionAtPosition(const C3DVector& pos);   // 0x00661D20
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;
    CThing* GetPItem();                     // 0x00A01B50
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* Creature;                       // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                       // +0x04
    unsigned char _pad[0xc];                // +0x08..+0x13
};

struct CAIStateGroup_MeleeCombatNearbyBase : CAIStateGroupBase {
    CBaseIntelligentPointer PEnemy;         // +0x14 CIntelligentPointer<CThingCreatureBase>
    bool IsStillRunnable(bool checkEnemy);  // 0x0093D800
};

struct CAIStateGroup_MeleeCombatNearby2 : CAIStateGroup_MeleeCombatNearbyBase {
    bool Archer;                            // +0x1c
    bool IsStillRunnable(bool checkEnemy);
};

bool CAIStateGroup_MeleeCombatNearby2::IsStillRunnable(bool checkEnemy)
{
    if (CAIStateGroup_MeleeCombatNearbyBase::IsStillRunnable(checkEnemy)) {
        CThing* enemy = PEnemy.GetPItem();
        CThing* creature = PBrain->Creature;
        long enemyRegion = enemy->GetRegionAtPosition(PEnemy.GetPItem()->GetPosition());
        long creatureRegion = creature->GetRegionAtPosition(creature->GetPosition());
        if (enemyRegion == creatureRegion)
            return true;
    }
    return false;
}