// CCombatAbilityBlockingData::CCombatAbilityBlockingData(const CCombatAbilityBlockingData&) -- copy constructor: the inlined CCombatAbilityData copy (its vtable, five dwords), own vtable, block angle, two flags, the weapon-class set copied out of line, the weapon class
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00739760, length 91 bytes.
// Callees: the std::set<EWeaponClass> copy constructor @ 0x00738DC0 (labelled "BinaryTree_Clone":
// allocates the head node and clones the source tree). Vtables: 0x01232484 = CCombatAbilityData
// (the inlined base copy constructor's dead store survives), 0x01232DEC = CCombatAbilityBlockingData.
// Retail layout (engine header +4 from +0x30 on, because the PDB set is 16 bytes): base
// CCombatAbilityData 0x18 {vptr, PercentageChance +4, SecondsDuration +8, SecondsToWaitBeforeRepeat
// +0xc, Anim +0x10, InterfaceType +0x14}, BlockAngle +0x18, BlockCanBeBroken +0x1c,
// ForceRecoilWhenBlocking +0x1d, ValidBlockWeaponTypes (set, 12 bytes) +0x20, WeaponClass +0x2c;
// size 0x30. Every scalar is copied with a plain move (the float too), so this is the memberwise
// copy; it is written out as an explicit member-initialiser copy constructor so the symbol is
// emitted out of line.
struct CCombatAbilityData_Retail {
    virtual void v00();
    long  PercentageChance;                                 // +0x04
    float SecondsDuration;                                  // +0x08
    float SecondsToWaitBeforeRepeat;                        // +0x0c
    long  Anim_TablePos;                                    // +0x10 CDefString
    long  InterfaceType;                                    // +0x14 ECombatAbilityInterfaceType
};

struct CRetailSet_EWeaponClass {                            // retail std::set: {head, size, (comparator)}
    void* head; unsigned long size; unsigned long comp;
    CRetailSet_EWeaponClass(const CRetailSet_EWeaponClass& other);   // 0x00738DC0
};

struct CCombatAbilityBlockingData_Retail : CCombatAbilityData_Retail {
    float BlockAngle;                                       // +0x18
    bool  BlockCanBeBroken;                                 // +0x1c
    bool  ForceRecoilWhenBlocking;                          // +0x1d
    CRetailSet_EWeaponClass ValidBlockWeaponTypes;          // +0x20
    long  WeaponClass;                                      // +0x2c EWeaponClass
    virtual void v00();                                     // own vtable 0x01232DEC
    CCombatAbilityBlockingData_Retail(const CCombatAbilityBlockingData_Retail& other);
};

CCombatAbilityBlockingData_Retail::CCombatAbilityBlockingData_Retail(const CCombatAbilityBlockingData_Retail& other)
    : CCombatAbilityData_Retail(other)
    , BlockAngle(other.BlockAngle)
    , BlockCanBeBroken(other.BlockCanBeBroken)
    , ForceRecoilWhenBlocking(other.ForceRecoilWhenBlocking)
    , ValidBlockWeaponTypes(other.ValidBlockWeaponTypes)
    , WeaponClass(other.WeaponClass)
{
}