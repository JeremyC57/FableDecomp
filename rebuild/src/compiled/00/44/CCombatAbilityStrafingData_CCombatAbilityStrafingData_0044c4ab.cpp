// CCombatAbilityStrafingData::CCombatAbilityStrafingData(ECombatAbilityInterfaceType interfaceType) -- CCombatAbilityData(interfaceType), then this class's vtable and RotationSpeed = 20.0f
// Leaf: ??0CCombatAbilityStrafingData@@QAE@W4ECombatAbilityInterfaceType@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0044C4AB, 31 bytes (size-optimised TU, odd entry), thiscall, `ret 4`. Vtable 0x012324AC.
// Identity: CCombatAbilityStrafingData::CCombatAbilityStrafingData(ECombatAbilityInterfaceType) -- proven; evidence: it stores vtable 0x012324AC whose RTTI is .?AVCCombatAbilityStrafingData@@ : CCombatAbilityData;
//   FableWin.pdb name set holds ??0CCombatAbilityStrafingData@@QAE@W4ECombatAbilityInterfaceType@@@Z; its only caller is the landed CCombatAbilityStrafeDef::CCombatAbilityStrafeDef 0x0044C57D (binds it as ??0CCombatAbilityStrafingData_Calls@@QAE@J@Z). Family G056 (seven such constructors; the propagated label
//   "CTCCameraCollision::CTCCameraCollision" is right only for 0x004DCA62).
// Callees: CCombatAbilityData::CCombatAbilityData(ECombatAbilityInterfaceType) 0x0044C3EE (landed callers bind ??0CCombatAbilityData@@QAE@J@Z; the PDB takes the enum (one dword either way)).
// Layout: Ego_r.pdb CCombatAbilityStrafingData::RotationSpeed (float) at retail +0x18.
#pragma optimize("s", on)
enum ECombatAbilityInterfaceType { COMBAT_ABILITY_INTERFACE_NULL };
class CCombatAbilityData {                                 // vtable 0x01232484, 0x18 bytes
public:
    CCombatAbilityData(ECombatAbilityInterfaceType interfaceType);   // 0x0044C3EE
    virtual ~CCombatAbilityData();
    long  PercentageChance;                                // +0x04
    float SecondsDuration;                                 // +0x08
    float SecondsToWaitBeforeRepeat;                       // +0x0c
    long  Anim;                                            // +0x10 CDefString
    long  InterfaceType;                                   // +0x14 ECombatAbilityInterfaceType
};
class CCombatAbilityStrafingData : public CCombatAbilityData {   // 0x1C bytes
public:
    CCombatAbilityStrafingData(ECombatAbilityInterfaceType interfaceType);
    float RotationSpeed;                                   // +0x18
};
CCombatAbilityStrafingData::CCombatAbilityStrafingData(ECombatAbilityInterfaceType interfaceType)
    : CCombatAbilityData(interfaceType), RotationSpeed(20.0f)
{
}