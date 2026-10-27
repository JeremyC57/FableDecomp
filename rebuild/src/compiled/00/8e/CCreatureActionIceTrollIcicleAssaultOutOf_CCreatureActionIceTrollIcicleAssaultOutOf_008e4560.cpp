// CCreatureAction_IceTrollIcicleAssaultOutOf::CCreatureAction_IceTrollIcicleAssaultOutOf(CThingCreatureBase& owner) -- the combat action for the owner: action code 150, an empty sound pair
// Leaf: ??0CCreatureAction_IceTrollIcicleAssaultOutOf@@QAE@AAVCThingCreatureBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E4560, 53 bytes, thiscall, `ret 4`. Vtable 0x012861EC.
// Identity (probable): it stores vtable 0x012861EC whose RTTI is .?AVCCreatureAction_IceTrollIcicleAssaultOutOf@@; the class is a TLC addition absent from Ego_r.pdb; the constructor signature is its combat-action siblings' (owner by reference); its caller is CModeIceTrollGroundSmashIcicleAssault::FrameUpdate 0x008DE7A0. The manifest's
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
class CCreatureAction_IceTrollIcicleAssaultOutOf : public CCombatActionBase {
public:
    virtual void v00();
    CCreatureAction_IceTrollIcicleAssaultOutOf(CThingCreatureBase& owner);
};
CCreatureAction_IceTrollIcicleAssaultOutOf::CCreatureAction_IceTrollIcicleAssaultOutOf(CThingCreatureBase& owner)
    : CCombatActionBase(owner, 150, CSoundPair())
{
}