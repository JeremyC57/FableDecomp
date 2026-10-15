// CCombatRing::AssignAttackerToProng(CProng* prong, CThingCreatureBase* attacker, CAttackingCombatantInfo* info, bool outerRing) -- put the attacker on the prong, grow the ring thickness to fit it, and have the combat manager unsheathe its weapon (type 5 on an outer ring, 4 otherwise)
// Retail: Fable.exe (TLC, VC7.1) @ 0x006EBEE0, 141 bytes (144 with padding).
// Callees: CProng::SetPOccupyingAttacker 0x006F17A0, CThingCreatureBase::GetCombatTypeDef 0x00666D10,
//          CThingCreatureBase::GetCombatManager 0x00661ED0, CCombatManager::CreatureUnsheathWeapon 0x006EBDD0.
//          Float pool 0x0125AADC = 1.5f.
//
// Shape: PDB CCombatRing (Index +8, RingThickness +0xc) and CAttackingCombatantInfo (CombatWheelRingIndex
// +8). An inner ring (Index != 0) is sized by twice the attacker's collision radius (the record at
// attacker +0x60, radius at +0x34: `fld ; fadd st,st`) times 1.5, the innermost ring by the combat type
// def's distance (+0x6c); both only grow the ring (`fcom ; test ah,41h ; jne` = `size > RingThickness`,
// the comparand kept on the FPU stack for the store). The weapon call is written in each arm (retail
// duplicates the whole call and return); its arguments are pushed before the GetCombatManager call.
struct CCombatTypeDef {
    unsigned char _pad[0x6c];
    float m_ringDistance;                  // +0x6c
};

struct SCollisionData {
    unsigned char _pad[0x34];
    float m_radius;                        // +0x34
};

struct CCombatManager;

struct CThingCreatureBase {
    unsigned char _pad[0x60];
    SCollisionData* m_pCollision;          // +0x60
    CCombatTypeDef* GetCombatTypeDef();    // 0x00666D10
    CCombatManager* GetCombatManager();    // 0x00661ED0
};

enum EUnsheatheWeaponType { UNSHEATHE_INNER_RING = 4, UNSHEATHE_OUTER_RING = 5 };

struct CCombatManager {
    void CreatureUnsheathWeapon(CThingCreatureBase* creature, EUnsheatheWeaponType type);   // 0x006EBDD0
};

struct CProng {
    void SetPOccupyingAttacker(CThingCreatureBase* attacker);   // 0x006F17A0
};

struct CAttackingCombatantInfo {
    CThingCreatureBase* Attacked;          // +0x00
    long AttackerNumber;                   // +0x04
    long CombatWheelRingIndex;             // +0x08
};

struct CCombatRing {
    float RotationAngle;                   // +0x00
    float DeltaRotation;                   // +0x04
    long Index;                            // +0x08
    float RingThickness;                   // +0x0c
    void AssignAttackerToProng(CProng* prong, CThingCreatureBase* attacker, CAttackingCombatantInfo* info, bool outerRing);
};

void CCombatRing::AssignAttackerToProng(CProng* prong, CThingCreatureBase* attacker, CAttackingCombatantInfo* info, bool outerRing)
{
    info->CombatWheelRingIndex = Index;
    prong->SetPOccupyingAttacker(attacker);
    if (Index != 0) {
        float size = attacker->m_pCollision->m_radius + attacker->m_pCollision->m_radius;
        if (size > RingThickness)
            RingThickness = size * 1.5f;
    } else {
        float size = attacker->GetCombatTypeDef()->m_ringDistance;
        if (size > RingThickness)
            RingThickness = size;
    }
    if (outerRing)
        attacker->GetCombatManager()->CreatureUnsheathWeapon(attacker, UNSHEATHE_OUTER_RING);
    else
        attacker->GetCombatManager()->CreatureUnsheathWeapon(attacker, UNSHEATHE_INNER_RING);
}
