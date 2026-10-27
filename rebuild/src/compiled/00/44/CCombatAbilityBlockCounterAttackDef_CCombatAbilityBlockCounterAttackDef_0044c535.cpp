// CCombatAbilityBlockCounterAttackDef::CCombatAbilityBlockCounterAttackDef() -- the CCombatAbilityAttackBase base built for type (ECombatAbilityInterfaceType)5, then this class's vtable
// Leaf: ??0CCombatAbilityBlockCounterAttackDef@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0044C535, 20 bytes, thiscall, plain `ret`, returns this.
// Identity (proven): stores vtable 0x0123253C (RTTI .?AVCCombatAbilityBlockCounterAttackDef@@ : CCombatAbilityAttackBase ...); FableWin.pdb ??0CCombatAbilityBlockCounterAttackDef@@QAE@XZ; the landed CCombatAbilityAttackBase ctor 0x0044C503 names these three derived attack-def constructors as its callers.
// Callees: CCombatAbilityAttackBase::CCombatAbilityAttackBase(ECombatAbilityInterfaceType) 0x0044C503.
// Shape (family K092, representative 0x0044C535): the base call with the constant pushed, the vtable store (size TU).
#pragma optimize("s", on)
enum ECombatAbilityInterfaceType { ECombatAbilityInterfaceType_Max = 0x7fffffff };   // enumerator names not known
class CCombatAbilityAttackBase {
public:
    CCombatAbilityAttackBase(ECombatAbilityInterfaceType type);                                  // 0x0044C503
    virtual ~CCombatAbilityAttackBase();
};
class CCombatAbilityBlockCounterAttackDef : public CCombatAbilityAttackBase {
public:
    CCombatAbilityBlockCounterAttackDef();
};
CCombatAbilityBlockCounterAttackDef::CCombatAbilityBlockCounterAttackDef()
    : CCombatAbilityAttackBase((ECombatAbilityInterfaceType)5)
{
}