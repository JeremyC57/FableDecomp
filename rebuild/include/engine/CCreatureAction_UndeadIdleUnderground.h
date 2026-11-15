#ifndef FABLE_ENGINE_CCREATUREACTION_UNDEADIDLEUNDERGROUND_H
#define FABLE_ENGINE_CCREATUREACTION_UNDEADIDLEUNDERGROUND_H
// CCreatureAction_UndeadIdleUnderground (agentC36, row 0x0074AF50): the action a freshly generated undead holds while it waits
// below ground. Ego_r.pdb / FableWin.pdb ??0CCreatureAction_UndeadIdleUnderground@@QAE@AAVCThingCreatureBase@@@Z (retail 0x0074DD20,
// out of line: CCombatActionBase(creature, 50, CSoundPair())), vtable 0x0126908C; no members of its own (0x114 bytes, the base).
// Needs FABLE_RETAIL_CCREATUREACTIONBASE and engine/CCreatureActionBase.h first, as engine/CCombatActionBase.h does.
#include "engine/CCombatActionBase.h"
class CThingCreatureBase;
class CCreatureAction_UndeadIdleUnderground : public CCombatActionBase {
public:
    CCreatureAction_UndeadIdleUnderground(CThingCreatureBase&);   // 0x0074DD20
};
#endif // FABLE_ENGINE_CCREATUREACTION_UNDEADIDLEUNDERGROUND_H
