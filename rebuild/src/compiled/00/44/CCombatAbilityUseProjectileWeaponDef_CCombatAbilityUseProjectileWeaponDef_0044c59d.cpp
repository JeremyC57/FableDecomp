// CCombatAbilityUseProjectileWeaponDef::CCombatAbilityUseProjectileWeaponDef() -- base CSubDefClassBase(), own vtable, then the CCombatAbilityProjectileAttackData member at +0x28 built with interface type 7
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0044C59D, length 28 bytes.
// Callees: CSubDefClassBase::CSubDefClassBase @ 0x0044C0C0 (vtable 0x01231D54; manifest label propagated),
// CCombatAbilityProjectileAttackData::CCombatAbilityProjectileAttackData(ECombatAbilityInterfaceType)
// @ 0x0044C4D5. PDB: CCombatAbilityUseProjectileWeaponDef = 0x48 bytes, `CombatAbility`
// (CCombatAbilityProjectileAttackData, 0x20 bytes) at +0x28. The by-value enum is a plain long.
// Vtable 0x012326DC from rtti-vtables.tsv.
struct CSubDefClassBase {
    virtual void v00();
    unsigned char _body[0x24];                              // +0x04 .. +0x28
    CSubDefClassBase();                                     // 0x0044C0C0
};

struct CCombatAbilityProjectileAttackData_Calls {           // engine/CCombatAbilityProjectileAttackData.h: 0x20 bytes
    virtual void v00();
    unsigned char _body[0x1c];
    CCombatAbilityProjectileAttackData_Calls(long interfaceType);   // 0x0044C4D5
};

struct CCombatAbilityUseProjectileWeaponDef : CSubDefClassBase {
    CCombatAbilityProjectileAttackData_Calls CombatAbility; // +0x28
    virtual void v00();                                     // own vtable 0x012326DC
    CCombatAbilityUseProjectileWeaponDef();
};

CCombatAbilityUseProjectileWeaponDef::CCombatAbilityUseProjectileWeaponDef()
    : CSubDefClassBase()
    , CombatAbility(7)
{
}