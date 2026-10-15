// CCombatAbilityBlockingData::CCombatAbilityBlockingData(ECombatAbilityInterfaceType, EWeaponClass) -- CCombatAbilityData(type), own vtable, a 120-degree block angle, block can be broken, force recoil when blocking, an empty valid-weapon set, the weapon class
// Leaf: ??0CCombatAbilityBlockingData
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00453DE0, length 54 bytes.
// Callees: CCombatAbilityData::CCombatAbilityData(ECombatAbilityInterfaceType) @ 0x0044C3EE (landed),
// set<EWeaponClass>::set() @ 0x00453572 (out of line in this size-optimised module; the manifest label
// "CDisplayManager::CopyBackBufferToTexture" is propagated). Vtable 0x01232DEC =
// CCombatAbilityBlockingData (rtti-vtables.tsv): the chunk label
// "CTCCreatureGeneratorCreator::CTCCreatureGeneratorCreator" is propagated.
// Layout = Ego_r.pdb CCombatAbilityBlockingData less 4 bytes from +0x2c (the retail set is 12 bytes):
// CCombatAbilityData 0x18, BlockAngle +0x18, BlockCanBeBroken +0x1c, ForceRecoilWhenBlocking +0x1d,
// ValidBlockWeaponTypes +0x20, WeaponClass +0x2c.
// Shape: `push dword ptr [esp+8]` (the argument pushed straight from its slot) and immediate byte stores
// mark a size-optimised module (`#pragma optimize("s", on)`); plain initialisers in declaration
// order, the weapon class stored after the set's constructor.
#pragma optimize("s", on)
struct CCombatAbilityData_Calls {                           // 0x18 bytes (vtable 0x01232484)
    virtual void v00();
    long  PercentageChance;                                 // +0x04
    float SecondsDuration;                                  // +0x08
    float SecondsToWaitBeforeRepeat;                        // +0x0c
    long  Anim;                                             // +0x10 CDefString
    long  InterfaceType;                                    // +0x14
    CCombatAbilityData_Calls(long interfaceType);           // 0x0044C3EE
};

struct CRetailSet_WeaponClass {                             // retail set<EWeaponClass>: 12 bytes
    void*         Header;
    unsigned long Count;
    unsigned char Compare[4];
    CRetailSet_WeaponClass();                               // 0x00453572
};

struct CCombatAbilityBlockingData : CCombatAbilityData_Calls {   // vtable 0x01232DEC
    float                  BlockAngle;                      // +0x18
    bool                   BlockCanBeBroken;                // +0x1c
    bool                   ForceRecoilWhenBlocking;         // +0x1d
    CRetailSet_WeaponClass ValidBlockWeaponTypes;           // +0x20
    long                   WeaponClass;                     // +0x2c EWeaponClass
    virtual void v00();
    CCombatAbilityBlockingData(long interfaceType, long weaponClass);
};

CCombatAbilityBlockingData::CCombatAbilityBlockingData(long interfaceType, long weaponClass)
    : CCombatAbilityData_Calls(interfaceType)
    , BlockAngle(120.0f)
    , BlockCanBeBroken(true)
    , ForceRecoilWhenBlocking(true)
    , ValidBlockWeaponTypes()
    , WeaponClass(weaponClass)
{
}
