#ifndef FABLE_ENGINE_CCREATUREACTION_RISEUP_H
#define FABLE_ENGINE_CCREATUREACTION_RISEUP_H
// New header (chunk Q19, row 0x008C4FD0 CCreatureAction_ScorpionSummon::CreateBabies): RTTI .?AVCCreatureAction_RiseUp@@
// (vtable 0x01268C9C) : CCombatActionBase. Ego_r.pdb CCreatureAction_RiseUp (sizeof 304 = CCombatActionBase 300 + FaceTarget bool
// +300); retail CCombatActionBase is 0x114, so FaceTarget sits at +0x114 and the class is 0x118 (CreateBabies builds the temporary in a
// 0x118-byte frame block). Ego_r.pdb ??0CCreatureAction_RiseUp@@QAE@AAVCThingCreatureBase@@@Z (retail 0x008BD880, out of line,
// `ret 4`; the two-argument (CThingCreatureBase&, CThing&) overload is the other Ego_r ctor). No user-declared destructor: the
// temporary's compiler-generated destructor calls ~CCombatActionBase 0x0062E900 (0x008C5249). Needs engine/CCombatActionBase.h
// (FABLE_RETAIL_CCREATUREACTIONBASE). No default path: the header is only included by rows that need the class.
#include "engine/CCombatActionBase.h"
class CThingCreatureBase;
class CThing;
class CCreatureAction_RiseUp : public CCombatActionBase {
public:
    CCreatureAction_RiseUp(CThingCreatureBase& creature);                    // 0x008BD880
    CCreatureAction_RiseUp(CThingCreatureBase& creature, CThing& target);    // (Ego_r; not bound here)
private:
    bool FaceTarget;                                                         // +0x114 (Ego_r +300)
};
FABLE_STATIC_ASSERT(sizeof(CCreatureAction_RiseUp) == 0x118);
#endif // FABLE_ENGINE_CCREATUREACTION_RISEUP_H
