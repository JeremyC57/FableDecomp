// CCombatAbilityBlockDefBase::CCombatAbilityBlockDefBase(ECombatAbilityInterfaceType, EWeaponClass) -- base CSubDefClassBase(), own vtable, then the CCombatAbilityBlockingData member at +0x28 built from both arguments
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00453E2D, length 36 bytes.
// Callees: CSubDefClassBase::CSubDefClassBase @ 0x0044C0C0 (vtable 0x01231D54; manifest label propagated),
// CCombatAbilityBlockingData::CCombatAbilityBlockingData(ECombatAbilityInterfaceType, EWeaponClass)
// @ 0x00453DE0 (labelled CTCCreatureGeneratorCreator; it builds CCombatAbilityData(type), 120-degree
// block angle, the two flags, the weapon-class set and stores the class at +0x2c).
// PDB: CCombatAbilityBlockDefBase = 0x5c bytes, `CombatAbility` at +0x28; retail CCombatAbilityBlockingData
// is 0x30 bytes (its std::set is 12 bytes, 4 less than Ego_r). Both arguments are pass-through
// pushes (`push dword ptr [esp+n]`), so the by-value enums are plain longs and the TU is size-optimised
// (`#pragma optimize("s", on)`). Vtable 0x01232E04.
#pragma optimize("s", on)
struct CSubDefClassBase {
    virtual void v00();
    unsigned char _body[0x24];                              // +0x04 .. +0x28
    CSubDefClassBase();                                     // 0x0044C0C0
};

struct CCombatAbilityBlockingData_Calls {                   // retail 0x30 bytes
    virtual void v00();
    unsigned char _body[0x2c];
    CCombatAbilityBlockingData_Calls(long interfaceType, long weaponClass);   // 0x00453DE0
};

struct CCombatAbilityBlockDefBase : CSubDefClassBase {
    CCombatAbilityBlockingData_Calls CombatAbility;         // +0x28
    virtual void v00();                                     // own vtable 0x01232E04
    CCombatAbilityBlockDefBase(long interfaceType, long weaponClass);
};

CCombatAbilityBlockDefBase::CCombatAbilityBlockDefBase(long interfaceType, long weaponClass)
    : CSubDefClassBase()
    , CombatAbility(interfaceType, weaponClass)
{
}