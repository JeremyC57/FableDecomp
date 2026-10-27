// CCombatAction_GenericResponseToDecapitation::CCombatAction_GenericResponseToDecapitation(CThingCreatureBase& owner) -- the combat action for the owner: action code 250, an empty sound pair
// Leaf: ??0CCombatAction_GenericResponseToDecapitation@@QAE@AAVCThingCreatureBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0066A560, 53 bytes, thiscall, `ret 4`. Vtable 0x0125AD94.
// Identity (proven): it stores vtable 0x0125AD94 whose RTTI is .?AVCCombatAction_GenericResponseToDecapitation@@; Ego_r.pdb ??0CCombatAction_GenericResponseToDecapitation@@QAE@AAVCThingCreatureBase@@@Z; callers CThingCreatureBase::ReactToGenericStrike 0x00667450 and CGameScriptInterface::EntityDecapitate 0x008A8E40. The manifest's
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
class CCombatAction_GenericResponseToDecapitation : public CCombatActionBase {
public:
    virtual void v00();
    CCombatAction_GenericResponseToDecapitation(CThingCreatureBase& owner);
};
CCombatAction_GenericResponseToDecapitation::CCombatAction_GenericResponseToDecapitation(CThingCreatureBase& owner)
    : CCombatActionBase(owner, 250, CSoundPair())
{
}