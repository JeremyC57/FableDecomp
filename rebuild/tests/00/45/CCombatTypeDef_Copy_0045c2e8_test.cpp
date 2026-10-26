#include <cstdio>
#include <cstring>
// Behaviour test: runs the reconstructed operator= on a filled source and checks every member and the out-of-line container/base calls.
// CCombatTypeDef::operator=(const CCombatTypeDef&) -- memberwise copy: CParentDefClassBase part, the ring-occupation, combat-sequence and sequence-def vectors, the distance limits, the attack-pattern map and the creature/recoil flags; returns *this

// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045C2E8, length 231 bytes.
// Pragma:  -- the def module is size-optimised (every copy goes through eax/al).
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
class CParentDefClassBase { public:
    virtual void v00() {}
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

class CCombatTypeDef : public CParentDefClassBase { public:   // vtable 0x01233744
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

static int g_baseCalls = 0, g_vecCalls = 0;
CParentDefClassBase& CParentDefClassBase::operator=(const CParentDefClassBase& o)
{ ++g_baseCalls; for (int i = 0; i < 0x35; ++i) _defClassBase[i] = o._defClassBase[i]; return *this; }
CLongVector& CLongVector::operator=(const CLongVector& o) { ++g_vecCalls; m_first = o.m_first; m_last = o.m_last; m_end = o.m_end; return *this; }
CStringVector& CStringVector::operator=(const CStringVector& o) { ++g_vecCalls; m_first = o.m_first; m_last = o.m_last; m_end = o.m_end; return *this; }
CRetailTree_0045c442& CRetailTree_0045c442::operator=(const CRetailTree_0045c442& o) { ++g_vecCalls; head = o.head; size = o.size; return *this; }

int main()
{
    static CCombatTypeDef a, b;
    std::memset(a._defClassBase, 0, sizeof(a._defClassBase));
    std::memset(b._defClassBase, 0, sizeof(b._defClassBase));
    b._defClassBase[0x21] = 0x5a;
    b.DefenderWheelOuterRingStartIndex = 7; b.DefenderWheelAllowAttackerStrafing = true; b.PreferOuterRing = true; b.FrontFarLimit = 1.5f; b.PreferredCombatDistance = 9.0f; b.CombatCreatureType = 3; b.KnockdownCausesStun = true; b.RecoilStyle = 2; b.CanBeKnockedDown = true; b.CombatFOV = 0.25f; b.StartsAsReadyForMelee = true; b.AttackPatterns.size = 4; b.CombatSequence.m_last = &b; b.CombatSequenceDef.m_end = (long*)&b;
    CCombatTypeDef& r = (a = b);
    bool ok = (&r == &a) && g_baseCalls == 1 && a._defClassBase[0x21] == 0x5a;
    ok = ok && g_vecCalls == 4 && a.DefenderWheelOuterRingStartIndex == 7 && a.DefenderWheelAllowAttackerStrafing && a.PreferOuterRing && a.FrontFarLimit == 1.5f && a.PreferredCombatDistance == 9.0f && a.CombatCreatureType == 3 && a.KnockdownCausesStun && !a.AlwaysAttackIfNearest && a.RecoilStyle == 2 && a.CanBeKnockedDown && a.CombatFOV == 0.25f && a.StartsAsReadyForMelee && a.AttackPatterns.size == 4 && a.CombatSequence.m_last == (void*)&b && a.CombatSequenceDef.m_end == (long*)&b;
    if (!ok) { std::printf("0045c2e8_TEST FAIL\n"); return 1; }
    std::printf("0045c2e8_TEST PASS\n");
    return 0;
}