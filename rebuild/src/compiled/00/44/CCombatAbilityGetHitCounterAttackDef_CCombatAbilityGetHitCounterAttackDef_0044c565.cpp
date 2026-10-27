// CCombatAbilityGetHitCounterAttackDef::CCombatAbilityGetHitCounterAttackDef() -- the CCombatAbilityAttackBase base built for type (ECombatAbilityInterfaceType)9, then this class's vtable
// Leaf: ??0CCombatAbilityGetHitCounterAttackDef@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0044C565, 20 bytes, thiscall, plain `ret`, returns this.
// Identity (proven): stores vtable 0x0123260C (RTTI .?AVCCombatAbilityGetHitCounterAttackDef@@ : CCombatAbilityAttackBase ...); FableWin.pdb ??0CCombatAbilityGetHitCounterAttackDef@@QAE@XZ; the landed CCombatAbilityAttackBase ctor 0x0044C503 names it as a caller.
// Callees: CCombatAbilityAttackBase::CCombatAbilityAttackBase(ECombatAbilityInterfaceType) 0x0044C503.
// Shape (family K092, representative 0x0044C535): the base call with the constant pushed, the vtable store (size TU).
#pragma optimize("s", on)
enum ECombatAbilityInterfaceType { ECombatAbilityInterfaceType_Max = 0x7fffffff };   // enumerator names not known
class CCombatAbilityAttackBase {
public:
    CCombatAbilityAttackBase(ECombatAbilityInterfaceType type);                                  // 0x0044C503
    virtual ~CCombatAbilityAttackBase();
};
class CCombatAbilityGetHitCounterAttackDef : public CCombatAbilityAttackBase {
public:
    CCombatAbilityGetHitCounterAttackDef();
};
CCombatAbilityGetHitCounterAttackDef::CCombatAbilityGetHitCounterAttackDef()
    : CCombatAbilityAttackBase((ECombatAbilityInterfaceType)9)
{
}