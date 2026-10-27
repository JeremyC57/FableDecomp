// CCreatureAction_RockTrollShakeAndExplode::CCreatureAction_RockTrollShakeAndExplode(CThingCreatureBase& owner) -- the combat action for the owner: action code 250, an empty sound pair
// Leaf: ??0CCreatureAction_RockTrollShakeAndExplode@@QAE@AAVCThingCreatureBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008C7BB0, 53 bytes, thiscall, `ret 4`. Vtable 0x0128154C.
// Identity (probable): it stores vtable 0x0128154C whose RTTI is .?AVCCreatureAction_RockTrollShakeAndExplode@@; the class is a TLC addition absent from Ego_r.pdb; the constructor signature is its combat-action siblings' (owner by reference). The manifest's
//   "CCombatAction_GenericResponseToDecapitation" is propagated across byte-identical bodies.
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase&, long, const CSoundPair&) 0x00858030.
// Shape (family M184, representative 0x0066A560): the landed combat-action constructor 0x008CBD50 -- the CSoundPair()
//   temporary zeroed through one register before the pushes, then this class's vtable. Speed TU.
class CThingCreatureBase;
class CSoundPair {
public:
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
class CCombatActionBase {
public:
    virtual void v00();
    CCombatActionBase(CThingCreatureBase& owner, long actionCode, const CSoundPair& sound);   // 0x00858030
};
class CCreatureAction_RockTrollShakeAndExplode : public CCombatActionBase {
public:
    virtual void v00();
    CCreatureAction_RockTrollShakeAndExplode(CThingCreatureBase& owner);
};
CCreatureAction_RockTrollShakeAndExplode::CCreatureAction_RockTrollShakeAndExplode(CThingCreatureBase& owner)
    : CCombatActionBase(owner, 250, CSoundPair())
{
}