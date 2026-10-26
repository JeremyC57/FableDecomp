// CAIStateGroup_MeleeCombatNearby2::GetProximityToBeConsideredNearToEnemyCreature() -- the distance at which
// the enemy counts as "near": the combat manager's proximity when the creature's combat-wheel sequence position
// is valid, otherwise twice the creature's def radius scaled by 5/6, plus one metre.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093DB70, length 86 bytes (+ int3 padding).
// Callees: CAIStateGroupBase::GetPWorld @ 0x008FCFC0 (twice), CCombatManager::CCombatWheel_ValidateSequencePosition
// (CThingCreatureBase*) @ 0x006E9700 (bool) and Combat_ProximityCalculation(CThingCreatureBase*) @ 0x006E9680
// (float in st0) on the world's combat manager at +0x4c. Pool constants: 0x012925CC = 0.8333333f (5/6),
// 0x0122DED8 = 1.0f.
// Shape: both branches read the creature into a named local BEFORE their world call (edi) and reach the combat
// manager through an INLINE ACCESSOR (`GetPWorld()->GetPCombatManager()`), which materialises the manager in eax
// with the creature push before the `mov ecx,eax` (a direct member chain pushes the creature before the world
// call); the float result of the manager call is returned straight from st0. The fallback reads the float at +0x34 of the creature's def pointer at +0x60:
// the doubling is written as `radius + radius` (`fadd st,st(0)`) -- `radius * 2.0f * K` lets VC fold the two
// literals into one `fmul` -- then `fmul [5/6]`, `fadd [1.0f]`. Base layout modelled locally.
struct CCreatureDefModel {
    unsigned char _pad[0x34];
    float m_radius;                         // +0x34
};

class CThingCreatureBase { public:
    unsigned char _pad[0x60];
    CCreatureDefModel* m_pDef;              // +0x60
};

class CCombatManager { public:
    bool CCombatWheel_ValidateSequencePosition(CThingCreatureBase* creature);   // 0x006E9700
    float Combat_ProximityCalculation(CThingCreatureBase* creature);            // 0x006E9680
};

class CWorld { public:
    unsigned char _pad[0x4c];
    CCombatManager* m_pCombatManager;       // +0x4c
    CCombatManager* GetPCombatManager() { return m_pCombatManager; }
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* Creature;           // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                       // +0x04
    unsigned char _pad[0xc];                // +0x08..+0x13
    CWorld* GetPWorld();                    // 0x008FCFC0
};

struct CAIStateGroup_MeleeCombatNearby2 : CAIStateGroupBase {
    unsigned char PEnemy[8];                // +0x14 CIntelligentPointer<CThingCreatureBase>
    bool Archer;                            // +0x1c
    float GetProximityToBeConsideredNearToEnemyCreature();
};

float CAIStateGroup_MeleeCombatNearby2::GetProximityToBeConsideredNearToEnemyCreature()
{
    CThingCreatureBase* creature = PBrain->Creature;
    if (GetPWorld()->GetPCombatManager()->CCombatWheel_ValidateSequencePosition(creature)) {
        CThingCreatureBase* combatant = PBrain->Creature;
        return GetPWorld()->GetPCombatManager()->Combat_ProximityCalculation(combatant);
    }
    float radius = PBrain->Creature->m_pDef->m_radius;
    return (radius + radius) * 0.8333333f + 1.0f;
}