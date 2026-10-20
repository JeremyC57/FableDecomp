// CHeroExperienceDef::operator=(const CHeroExperienceDef&) -- memberwise def assignment: CDefClassBase part, three vector<long>, the seven maps, two floats and the attack-history combo vector; returns *this
// Leaf: ??4CHeroExperienceDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004F914A, length 189 bytes; size-optimised def TU.
// Callees: CDefClassBase::operator= @ 0x00431F10, vector<long>::operator= @ 0x00454886 (x3),
//          map<long,CDefString>::operator= @ 0x004F9207 (x5), map<long,CComboMultiplierData>::operator=
//          @ 0x004F9660, map<EDamageAttribute,CHeroStatIncreaseData>::operator= @ 0x004F9831,
//          vector<CAttackHistoryCombo>::operator= @ 0x004F932C.
// Identity: the manifest label "CThingBuildingDef::operator=" is propagated over every def assignment.
// The body is reached through the `jmp` thunk at 0x004F9145 in slot 19 of vtable 0x0124390C =
// CHeroExperienceDef (rtti-vtables.tsv). PDB CHeroExperienceDef (Ego_r) puts its members at
// +0x28.. with 16-byte containers; retail STLport/Dinkumware containers are 12 bytes, so the
// members sit at +0x28/+0x34/+0x40 (vectors), +0x4c/+0x58 (maps), +0x64/+0x68 (floats),
// +0x6c/+0x78/+0x84/+0x90/+0x9c (maps) and +0xa8 (vector), exactly the offsets the body uses.
// The tree assignments keep the class names their landed bodies carry (CRetailTree_<copy fn>).
// Shape: the compiler-generated memberwise assignment written out in declaration order; the two
// floats copy through eax (single float member assignment is an integer move), the last member's
// address is formed with `add edi,0A8h` because nothing reads `other` after it.
// The generated engine headers flatten the bases (no base operator= to call), so the class is local.
#pragma optimize("s", on)   // the def module is size-optimised

struct CDefClassBase {
    unsigned char _body[0x28];          // vptr, resource links, PDefManager, GlobalIndex, flag bits
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};

struct CLongVector {                    // retail std::vector<long> (12 bytes)
    long* m_first;
    long* m_last;
    long* m_end;
    CLongVector& operator=(const CLongVector& other);   // 0x00454886
};

struct CRetailTree_004f927a {           // map<long, CDefString> (head + size + empty compare, 12 bytes)
    void* head;
    unsigned long size;
    unsigned long _compare;
    CRetailTree_004f927a& operator=(const CRetailTree_004f927a& other);   // 0x004F9207
};

struct CRetailTree_004f96d3 {           // map<long, CComboMultiplierData>
    void* head;
    unsigned long size;
    unsigned long _compare;
    CRetailTree_004f96d3& operator=(const CRetailTree_004f96d3& other);   // 0x004F9660
};

struct CRetailTree_004f98a4 {           // map<EDamageAttribute, CHeroStatIncreaseData>
    void* head;
    unsigned long size;
    unsigned long _compare;
    CRetailTree_004f98a4& operator=(const CRetailTree_004f98a4& other);   // 0x004F9831
};

struct CAttackHistoryComboVector {      // retail std::vector<CAttackHistoryCombo> (12 bytes)
    void* m_first;
    void* m_last;
    void* m_end;
    CAttackHistoryComboVector& operator=(const CAttackHistoryComboVector& other);   // 0x004F932C
};

struct CHeroExperienceDef : CDefClassBase {                         // vtable 0x0124390C
    CLongVector ExperienceLevelThresholds;                          // +0x28
    CLongVector HeroLevelTitles;                                    // +0x34
    CLongVector FallbackMultipliers;                                // +0x40
    CRetailTree_004f927a SoundIntensityThresholds;                  // +0x4c
    CRetailTree_004f96d3 ComboMultiplierMap;                        // +0x58
    float ExperienceMultiplierParamPerUnitComboMultiplier;          // +0x64
    float ExperienceMultiplierRegionChangeMultiplier;               // +0x68
    CRetailTree_004f927a ComboMultiplierIncrease;                   // +0x6c
    CRetailTree_004f927a ComboMultiplierLock;                       // +0x78
    CRetailTree_004f927a ComboMultiplierLost;                       // +0x84
    CRetailTree_004f927a ComboMultiplierRunningHitsIncrease;        // +0x90
    CRetailTree_004f98a4 StatIncreasePerHitType;                    // +0x9c
    CAttackHistoryComboVector AttackHistoryCombos;                  // +0xa8
    CHeroExperienceDef& operator=(const CHeroExperienceDef& other);
};

CHeroExperienceDef& CHeroExperienceDef::operator=(const CHeroExperienceDef& other)
{
    CDefClassBase::operator=(other);
    ExperienceLevelThresholds = other.ExperienceLevelThresholds;
    HeroLevelTitles = other.HeroLevelTitles;
    FallbackMultipliers = other.FallbackMultipliers;
    SoundIntensityThresholds = other.SoundIntensityThresholds;
    ComboMultiplierMap = other.ComboMultiplierMap;
    ExperienceMultiplierParamPerUnitComboMultiplier = other.ExperienceMultiplierParamPerUnitComboMultiplier;
    ExperienceMultiplierRegionChangeMultiplier = other.ExperienceMultiplierRegionChangeMultiplier;
    ComboMultiplierIncrease = other.ComboMultiplierIncrease;
    ComboMultiplierLock = other.ComboMultiplierLock;
    ComboMultiplierLost = other.ComboMultiplierLost;
    ComboMultiplierRunningHitsIncrease = other.ComboMultiplierRunningHitsIncrease;
    StatIncreasePerHitType = other.StatIncreasePerHitType;
    AttackHistoryCombos = other.AttackHistoryCombos;
    return *this;
}