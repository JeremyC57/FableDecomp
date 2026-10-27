// CCombatAbilityBlockLightWeaponAttackDef::CCombatAbilityBlockLightWeaponAttackDef() -- the block-def base built for interface type 2 and weapon class 1, then this class's vtable
// Leaf: ??0CCombatAbilityBlockLightWeaponAttackDef@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00453E84, 22 bytes, thiscall, plain `ret`, returns this.
// Identity (proven): stores vtable 0x01232ED4 (RTTI .?AVCCombatAbilityBlockLightWeaponAttackDef@@ : CCombatAbilityBlockDefBase ...); FableWin.pdb ??0CCombatAbilityBlockLightWeaponAttackDef@@QAE@XZ;
//   the four block attack defs sit in a row (0x00453E6A..0x00453EB8) right after their landed base constructor.
// Callees: CCombatAbilityBlockDefBase::CCombatAbilityBlockDefBase(ECombatAbilityInterfaceType, EWeaponClass) 0x00453E2D.
// Shape (family K093, representative 0x00453E6A): the two constants pushed right to left, the base call, the vtable store.
#pragma optimize("s", on)
enum ECombatAbilityInterfaceType { ECombatAbilityInterfaceType_Max = 0x7fffffff };   // enumerator names not known
enum EWeaponClass { EWeaponClass_Max = 0x7fffffff };
class CCombatAbilityBlockDefBase {
public:
    CCombatAbilityBlockDefBase(ECombatAbilityInterfaceType type, EWeaponClass weaponClass);   // 0x00453E2D
    virtual ~CCombatAbilityBlockDefBase();
};
class CCombatAbilityBlockLightWeaponAttackDef : public CCombatAbilityBlockDefBase {
public:
    CCombatAbilityBlockLightWeaponAttackDef();
};
CCombatAbilityBlockLightWeaponAttackDef::CCombatAbilityBlockLightWeaponAttackDef()
    : CCombatAbilityBlockDefBase((ECombatAbilityInterfaceType)2, (EWeaponClass)1)
{
}