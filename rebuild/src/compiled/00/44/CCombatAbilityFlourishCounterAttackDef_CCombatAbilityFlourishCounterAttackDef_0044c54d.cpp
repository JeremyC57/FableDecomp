// CCombatAbilityFlourishCounterAttackDef::CCombatAbilityFlourishCounterAttackDef() -- the CCombatAbilityAttackBase base built for type (ECombatAbilityInterfaceType)6, then this class's vtable
// Leaf: ??0CCombatAbilityFlourishCounterAttackDef@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0044C54D, 20 bytes, thiscall, plain `ret`, returns this.
// Identity (proven): stores vtable 0x012325A4 (RTTI .?AVCCombatAbilityFlourishCounterAttackDef@@ : CCombatAbilityAttackBase ...); FableWin.pdb ??0CCombatAbilityFlourishCounterAttackDef@@QAE@XZ; the landed CCombatAbilityAttackBase ctor 0x0044C503 names it as a caller.
// Callees: CCombatAbilityAttackBase::CCombatAbilityAttackBase(ECombatAbilityInterfaceType) 0x0044C503.
// Shape (family K092, representative 0x0044C535): the base call with the constant pushed, the vtable store (size TU).
#pragma optimize("s", on)
enum ECombatAbilityInterfaceType { ECombatAbilityInterfaceType_Max = 0x7fffffff };   // enumerator names not known
class CCombatAbilityAttackBase {
public:
    CCombatAbilityAttackBase(ECombatAbilityInterfaceType type);                                  // 0x0044C503
    virtual ~CCombatAbilityAttackBase();
};
class CCombatAbilityFlourishCounterAttackDef : public CCombatAbilityAttackBase {
public:
    CCombatAbilityFlourishCounterAttackDef();
};
CCombatAbilityFlourishCounterAttackDef::CCombatAbilityFlourishCounterAttackDef()
    : CCombatAbilityAttackBase((ECombatAbilityInterfaceType)6)
{
}