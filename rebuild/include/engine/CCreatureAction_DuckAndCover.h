#ifndef FABLE_ENGINE_CCREATUREACTION_DUCKANDCOVER_H
#define FABLE_ENGINE_CCREATUREACTION_DUCKANDCOVER_H
// New header (chunk Q17, row 0x009711D0). CCreatureAction_DuckAndCover : CCombatActionBase (RTTI, own vtable 0x012974A4; retail
// sizeof 0x120: the base is 0x114 and the ctor 0x009717A0 copies the position into +0x114..+0x11C). Ego_r.pdb
// ??0CCreatureAction_DuckAndCover@@QAE@AAVCThingCreatureBase@@ABVC3DVector@@@Z (the creature by REFERENCE; `ret 8`). No user-declared
// destructor: the temporary's compiler-generated destructor calls ~CCombatActionBase 0x0062E900 (0x009712F1). The position member
// is Ego_r.pdb FacingDirection (C3DVector, Ego_r +0x12C = retail +0x114).
// Needs engine/CCombatActionBase.h (its retail branch: FABLE_RETAIL_CCREATUREACTIONBASE) and engine/C3DVector.h.
#include "engine/CCombatActionBase.h"
#include "engine/C3DVector.h"
class CThingCreatureBase;
class CCreatureAction_DuckAndCover : public CCombatActionBase {
public:
    CCreatureAction_DuckAndCover(CThingCreatureBase& creature, const C3DVector& pos);   // 0x009717A0
private:
    C3DVector FacingDirection;                                           // +0x114
};
FABLE_STATIC_ASSERT(sizeof(CCreatureAction_DuckAndCover) == 0x120);
#endif
