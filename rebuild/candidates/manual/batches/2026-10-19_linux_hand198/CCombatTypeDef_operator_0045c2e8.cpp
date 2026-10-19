// CCombatTypeDef::operator=(const CCombatTypeDef&) -- memberwise copy: CParentDefClassBase part, the ring-occupation, combat-sequence and sequence-def vectors, the distance limits, the attack-pattern map and the creature/recoil flags; returns *this
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045C2E8, length 231 bytes.
// Pragma: #pragma optimize("s", on) -- the def module is size-optimised (every copy goes through eax/al).
// Callees: CParentDefClassBase::operator= @ 0x00431ED0, vector<long>::operator= @ 0x00454886 (twice),
//          vector<CCharString>::operator= @ 0x00432EE9, map<ECombatCreatureType,long>::operator= @ 0x0045C3CF
//          (the landed tree's CRetailTree_0045c442, named after its Copy helper).
// Identity: the catalog label "CParentDefClassBase::operator=" is propagated. Slot 19 (Copy) of
// CCombatTypeDef's vtable 0x01233744 is the 5-byte `jmp 0x0045C2E8` right before this body. The PDB
// layout (engine/CCombatTypeDef.h) applies once the retail containers shrink to 12 bytes and the base
// to 0x3c: PDB +0x40 -> retail +0x3c, and every container after that moves up another 4.
// Shape: memberwise assignment in declaration order; containers through their out-of-line operator=,
// scalars as dword/byte moves.
#pragma pack(push, 1)
struct CParentDefClassBase {
    virtual void v00();
    unsigned char _defClassBase[0x35];      // +0x04 CDefClassBase part, InstantiationName (+0x25), SubDefInfoMap (+0x29)
    CParentDefClassBase& operator=(const CParentDefClassBase& other);   // 0x00431ED0
};
#pragma pack(pop)

struct CLongVector {                    // retail std::vector<long> (12 bytes)
    long* m_first;
    long* m_last;
    long* m_end;
    CLongVector& operator=(const CLongVector& other);   // 0x00454886
};

struct CStringVector {                  // retail std::vector<CCharString> (12 bytes)
    void* m_first;
    void* m_last;
    void* m_end;
    CStringVector& operator=(const CStringVector& other);   // 0x00432EE9
};

struct CRetailTree_0045c442 {           // retail std::map<ECombatCreatureType,long> (12 bytes)
    void* head;
    unsigned long size;
    unsigned long _comp;
    CRetailTree_0045c442& operator=(const CRetailTree_0045c442& other);   // 0x0045C3CF
};

struct CCombatTypeDef : CParentDefClassBase {   // vtable 0x01233744
    CLongVector          DefenderCombatRingOccupationNumbers;  // +0x3c
    long                 DefenderWheelOuterRingStartIndex;     // +0x48
    bool                 DefenderWheelAllowAttackerStrafing;   // +0x4c
    CStringVector        CombatSequence;                       // +0x50
    CLongVector          CombatSequenceDef;                    // +0x5c
    bool                 PreferOuterRing;                      // +0x68
    float                FrontFarLimit;                        // +0x6c
    float                FrontMiddleLimit;                     // +0x70
    float                FrontNearLimit;                       // +0x74
    float                SideLimit;                            // +0x78
    float                RearLimit;                            // +0x7c
    float                PreferredCombatDistance;              // +0x80
    CRetailTree_0045c442 AttackPatterns;                       // +0x84
    long                 CombatCreatureType;                   // +0x90 ECombatCreatureType
    bool                 AlwaysAttackIfNearest;                // +0x94
    bool                 KnockdownCausesStun;                  // +0x95
    long                 RecoilStyle;                          // +0x98 ECombatStrikeRecoilStyle
    bool                 CanBeKnockedDown;                     // +0x9c
    float                ComboContinueSpeed;                   // +0xa0
    float                CombatFOV;                            // +0xa4
    bool                 StartsAsReadyForMelee;                // +0xa8
    CCombatTypeDef& operator=(const CCombatTypeDef& other);
};

CCombatTypeDef& CCombatTypeDef::operator=(const CCombatTypeDef& other)
{
    CParentDefClassBase::operator=(other);
    DefenderCombatRingOccupationNumbers = other.DefenderCombatRingOccupationNumbers;
    DefenderWheelOuterRingStartIndex = other.DefenderWheelOuterRingStartIndex;
    DefenderWheelAllowAttackerStrafing = other.DefenderWheelAllowAttackerStrafing;
    CombatSequence = other.CombatSequence;
    CombatSequenceDef = other.CombatSequenceDef;
    PreferOuterRing = other.PreferOuterRing;
    FrontFarLimit = other.FrontFarLimit;
    FrontMiddleLimit = other.FrontMiddleLimit;
    FrontNearLimit = other.FrontNearLimit;
    SideLimit = other.SideLimit;
    RearLimit = other.RearLimit;
    PreferredCombatDistance = other.PreferredCombatDistance;
    AttackPatterns = other.AttackPatterns;
    CombatCreatureType = other.CombatCreatureType;
    AlwaysAttackIfNearest = other.AlwaysAttackIfNearest;
    KnockdownCausesStun = other.KnockdownCausesStun;
    RecoilStyle = other.RecoilStyle;
    CanBeKnockedDown = other.CanBeKnockedDown;
    ComboContinueSpeed = other.ComboContinueSpeed;
    CombatFOV = other.CombatFOV;
    StartsAsReadyForMelee = other.StartsAsReadyForMelee;
    return *this;
}
