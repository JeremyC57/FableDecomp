// CCombatAbilityBlockingData::operator=(const CCombatAbilityBlockingData&) -- memberwise assignment: CCombatAbilityData::operator= out of line, block angle, two flags, the weapon-class set assigned out of line, the weapon class
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00461BB7, length 55 bytes.
// Callees: CCombatAbilityData::operator= @ 0x00453F18 (five dword copies), std::set<EWeaponClass>::
// operator= @ 0x00461BF0 (landed as CRetailTree00461c63_operator_00461bf0: clear + clone).
// Retail layout (engine header +4 from +0x30 on, because the PDB set is 16 bytes): base
// CCombatAbilityData 0x18, BlockAngle +0x18, BlockCanBeBroken +0x1c, ForceRecoilWhenBlocking +0x1d,
// ValidBlockWeaponTypes (12-byte set) +0x20, WeaponClass +0x2c; size 0x30 -- hence `_Retail`. The
// float is copied with a plain dword move; the operator is spelled out memberwise so the symbol is
// emitted out of line and returns *this. The un-interleaved byte copies through `al` and the
// `pop edi` after the last store are the size optimiser's schedule (`#pragma optimize("s", on)`),
// as in the landed set operator= of the same TU.
#pragma optimize("s", on)
struct CCombatAbilityData_Retail {
    virtual void v00();
    long  PercentageChance;                                 // +0x04
    float SecondsDuration;                                  // +0x08
    float SecondsToWaitBeforeRepeat;                        // +0x0c
    long  Anim_TablePos;                                    // +0x10 CDefString
    long  InterfaceType;                                    // +0x14
    CCombatAbilityData_Retail& operator=(const CCombatAbilityData_Retail& other);   // 0x00453F18
};

struct CRetailSet_EWeaponClass {                            // retail std::set: {head, size, (comparator)}
    void* head; unsigned long size; unsigned long comp;
    CRetailSet_EWeaponClass& operator=(const CRetailSet_EWeaponClass& other);   // 0x00461BF0
};

struct CCombatAbilityBlockingData_Retail : CCombatAbilityData_Retail {
    float BlockAngle;                                       // +0x18
    bool  BlockCanBeBroken;                                 // +0x1c
    bool  ForceRecoilWhenBlocking;                          // +0x1d
    CRetailSet_EWeaponClass ValidBlockWeaponTypes;          // +0x20
    long  WeaponClass;                                      // +0x2c EWeaponClass
    virtual void v00();
    CCombatAbilityBlockingData_Retail& operator=(const CCombatAbilityBlockingData_Retail& other);
};

CCombatAbilityBlockingData_Retail& CCombatAbilityBlockingData_Retail::operator=(const CCombatAbilityBlockingData_Retail& other)
{
    CCombatAbilityData_Retail::operator=(other);
    BlockAngle = other.BlockAngle;
    BlockCanBeBroken = other.BlockCanBeBroken;
    ForceRecoilWhenBlocking = other.ForceRecoilWhenBlocking;
    ValidBlockWeaponTypes = other.ValidBlockWeaponTypes;
    WeaponClass = other.WeaponClass;
    return *this;
}
