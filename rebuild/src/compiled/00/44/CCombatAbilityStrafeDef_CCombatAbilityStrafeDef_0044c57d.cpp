// CCombatAbilityStrafeDef::CCombatAbilityStrafeDef() -- base CSubDefClassBase(), own vtable, then the CCombatAbilityStrafingData member at +0x28 built with interface type 8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0044C57D, length 28 bytes.
// Callees: CSubDefClassBase::CSubDefClassBase @ 0x0044C0C0 (stores vtable 0x01231D54 = CSubDefClassBase;
// the manifest's "CShoveHaPennyRegionDef" label is propagated), CCombatAbilityStrafingData::
// CCombatAbilityStrafingData(ECombatAbilityInterfaceType) @ 0x0044C4AB (labelled CTCCameraCollision).
// Layout (PDB, engine/CCombatAbilityStrafeDef.h agrees): the 0x28-byte CSubDefClassBase, then the
// 0x1c-byte CCombatAbilityStrafingData `CombatAbility` at +0x28. The header is a flat POD, so a
// `_Retail` model with the real base/member classes is used; the by-value enum is a plain long (8 =
// the strafe ability's interface type). Vtable 0x01232674 from rtti-vtables.tsv.
class CSubDefClassBase { public:
    virtual void v00();
    unsigned char _body[0x24];                              // +0x04 .. +0x28
    CSubDefClassBase();                                     // 0x0044C0C0
};

struct CCombatAbilityStrafingData_Calls {                   // engine/CCombatAbilityStrafingData.h: 0x1c bytes
    virtual void v00();
    unsigned char _body[0x18];
    CCombatAbilityStrafingData_Calls(long interfaceType);   // 0x0044C4AB
};

struct CCombatAbilityStrafeDef_Retail : CSubDefClassBase {
    CCombatAbilityStrafingData_Calls CombatAbility;         // +0x28
    virtual void v00();                                     // own vtable 0x01232674
    CCombatAbilityStrafeDef_Retail();
};

CCombatAbilityStrafeDef_Retail::CCombatAbilityStrafeDef_Retail()
    : CSubDefClassBase()
    , CombatAbility(8)
{
}