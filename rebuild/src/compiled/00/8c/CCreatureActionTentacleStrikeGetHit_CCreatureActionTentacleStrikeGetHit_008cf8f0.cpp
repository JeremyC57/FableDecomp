// CCreatureAction_TentacleStrikeGetHit::CCreatureAction_TentacleStrikeGetHit() -- the CCombatActionBase base built for type 150, then this class's vtable
// Leaf: ??0CCreatureAction_TentacleStrikeGetHit@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008CF8F0, 23 bytes, thiscall, plain `ret`, returns this.
// Identity (proven): stores vtable 0x01285184 (RTTI .?AVCCreatureAction_TentacleStrikeGetHit@@ : CCombatActionBase ...); FableWin.pdb ??0CCreatureAction_TentacleStrikeGetHit@@QAE@XZ; its caller is the action factory 0x006F5C10; the action type 150 goes to the landed CCombatActionBase(long) 0x00857E40 (bound public; Ego_r.pdb has it protected).
// Callees: CCombatActionBase::CCombatActionBase(long) 0x00857E40.
// Shape (family K092, representative 0x0044C535): the base call with the constant pushed, the vtable store (size TU).
#pragma optimize("s", on)
class CCombatActionBase {
public:
    CCombatActionBase(long type);                                  // 0x00857E40
    virtual ~CCombatActionBase();
};
class CCreatureAction_TentacleStrikeGetHit : public CCombatActionBase {
public:
    CCreatureAction_TentacleStrikeGetHit();
};
CCreatureAction_TentacleStrikeGetHit::CCreatureAction_TentacleStrikeGetHit()
    : CCombatActionBase(150)
{
}