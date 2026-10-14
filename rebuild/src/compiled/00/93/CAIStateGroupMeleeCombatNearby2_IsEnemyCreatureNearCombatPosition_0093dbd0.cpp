// CAIStateGroup_MeleeCombatNearby2::IsEnemyCreatureNearCombatPosition() -- for a registered combatant, ask the
// combat manager whether the creature is within ring range of its target.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093DBD0, length 56 bytes (+ int3 padding).
// Callees: the creature's IsRegisteredCombatant query @ 0x00662410 (no-argument bool), CAIStateGroupBase::
// GetPWorld @ 0x008FCFC0, CCombatManager::Combat_CheckRingRangeToTarget(CThingCreatureBase*) @ 0x006E94D0 on
// the world's combat manager at +0x4c.
// Shape: `if (a()) { creature local; if (GetPWorld()->mgr->Check(creature)) return true; } return false;` --
// the creature is a named local read before the world call (edi) and the combat manager is reached through an
// INLINE ACCESSOR (`GetPWorld()->GetPCombatManager()->...`): the accessor materialises the manager in eax and
// the creature push lands between it and the `mov ecx,eax` (a direct member chain pushes the creature before
// the world call and loads the manager straight into ecx). One shared `xor al,al` false tail.
// Base layout modelled locally (PDB names).
struct CThingCreatureBase {
    bool IsRegisteredCombatant();           // 0x00662410
};

struct CCombatManager {
    bool Combat_CheckRingRangeToTarget(CThingCreatureBase* creature);   // 0x006E94D0
};

struct CWorld {
    unsigned char _pad[0x4c];
    CCombatManager* m_pCombatManager;       // +0x4c
    CCombatManager* GetPCombatManager() { return m_pCombatManager; }
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* Creature;           // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                       // +0x04
    unsigned char _pad[0xc];                // +0x08..+0x13
    CWorld* GetPWorld();                    // 0x008FCFC0
};

struct CAIStateGroup_MeleeCombatNearby2 : CAIStateGroupBase {
    unsigned char PEnemy[8];                // +0x14 CIntelligentPointer<CThingCreatureBase>
    bool Archer;                            // +0x1c
    bool IsEnemyCreatureNearCombatPosition();
};

bool CAIStateGroup_MeleeCombatNearby2::IsEnemyCreatureNearCombatPosition()
{
    if (PBrain->Creature->IsRegisteredCombatant()) {
        CThingCreatureBase* creature = PBrain->Creature;
        if (GetPWorld()->GetPCombatManager()->Combat_CheckRingRangeToTarget(creature))
            return true;
    }
    return false;
}