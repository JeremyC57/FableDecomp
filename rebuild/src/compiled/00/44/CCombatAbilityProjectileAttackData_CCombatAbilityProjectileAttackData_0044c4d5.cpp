// CCombatAbilityProjectileAttackData::CCombatAbilityProjectileAttackData(ECombatAbilityInterfaceType) -- the CCombatAbilityData base, this class's vtable, then AbleToStrafeAndShoot = false, ShotAccuracyPercentage = 30
// Leaf: ??0CCombatAbilityProjectileAttackData@@QAE@W4ECombatAbilityInterfaceType@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0044C4D5, 35 bytes, thiscall, `ret 4`, returns this.
// Identity (proven): stores vtable 0x012324C0 (RTTI .?AVCCombatAbilityProjectileAttackData@@ : CCombatAbilityData ...); FableWin.pdb ??0CCombatAbilityProjectileAttackData@@QAE@W4ECombatAbilityInterfaceType@@@Z; only caller
//   CCombatAbilityUseProjectileWeaponDef::CCombatAbilityUseProjectileWeaponDef 0x0044C59D (landed); the stores are Ego_r.pdb's CCombatAbilityProjectileAttackData members (retail = Ego_r.pdb).
// Callees: CCombatAbilityData::CCombatAbilityData(ECombatAbilityInterfaceType) 0x0044C3EE.
// Shape (family K091, representative 0x0044C4D5): the argument forwarded to the base, the vtable store, the two member
//   initialisers in declaration order; size TU (the pass-through `push dword ptr [esp+8]`).
#pragma optimize("s", on)
enum ECombatAbilityInterfaceType { };                   // enumerators not needed here
class CCombatAbilityData {                                            // retail 0x18 bytes (members not touched here)
public:
    CCombatAbilityData(ECombatAbilityInterfaceType arg);                                   // 0x0044C3EE
    virtual ~CCombatAbilityData();
    unsigned char _body[0x18 - 4];
};
class CCombatAbilityProjectileAttackData : public CCombatAbilityData {
public:
    CCombatAbilityProjectileAttackData(ECombatAbilityInterfaceType arg);
    bool      AbleToStrafeAndShoot;                // +0x18
    long      ShotAccuracyPercentage;              // +0x1C
};
CCombatAbilityProjectileAttackData::CCombatAbilityProjectileAttackData(ECombatAbilityInterfaceType arg)
    : CCombatAbilityData(arg), AbleToStrafeAndShoot(false), ShotAccuracyPercentage(30)
{
}