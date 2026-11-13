#ifndef FABLE_ENGINE_CCREATUREACTION_TROLLGROUNDPOUND_H
#define FABLE_ENGINE_CCREATUREACTION_TROLLGROUNDPOUND_H
#include "engine/CCombatActionBase.h"
// Both PDBs: no added fields; full native size0x114, original base virtuals.
class CCreatureAction_TrollGroundPound : public CCombatActionBase {
public:
#ifdef FABLE_RETAIL_CCREATUREACTION_TROLLGROUNDPOUND_CTOR
 // Opt-in (chunk 2026, row 0x0094E7B0): Ego_r.pdb / FableWin.pdb ??0CCreatureAction_TrollGroundPound@@QAE@AAVCThingCreatureBase@@0@Z,
 // out of line in retail at 0x0094E5F0 (landed; `ret 8`). Declaration only.
 CCreatureAction_TrollGroundPound(CThingCreatureBase& creature, CThingCreatureBase& target);   // 0x0094E5F0
#endif
 virtual void Initialise();
 virtual void FinishAction();
};
#endif
