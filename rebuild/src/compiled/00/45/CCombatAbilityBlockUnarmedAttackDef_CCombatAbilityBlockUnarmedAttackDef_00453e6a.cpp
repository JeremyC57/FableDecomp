// CCombatAbilityBlockUnarmedAttackDef::CCombatAbilityBlockUnarmedAttackDef() -- the block-def base built for interface type 1 and weapon class 0, then this class's vtable
// Leaf: ??0CCombatAbilityBlockUnarmedAttackDef@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00453E6A, 22 bytes, thiscall, plain `ret`, returns this.
// Identity (proven): stores vtable 0x01232E6C (RTTI .?AVCCombatAbilityBlockUnarmedAttackDef@@ : CCombatAbilityBlockDefBase ...); FableWin.pdb ??0CCombatAbilityBlockUnarmedAttackDef@@QAE@XZ;
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
class CCombatAbilityBlockUnarmedAttackDef : public CCombatAbilityBlockDefBase {
public:
    CCombatAbilityBlockUnarmedAttackDef();
};
CCombatAbilityBlockUnarmedAttackDef::CCombatAbilityBlockUnarmedAttackDef()
    : CCombatAbilityBlockDefBase((ECombatAbilityInterfaceType)1, (EWeaponClass)0)
{
}