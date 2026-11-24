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
#ifdef FABLE_RETAIL_CCREATUREACTION_UNDEADIDLEUNDERGROUND_RTTI_DECLS
    // Opt-in (NF324, 2026-11-24): declarations only, no layout change. Each member below is the one the retail RTTI vftable placement
    // (ghidra_out/labels_rtti_port.tsv) names at its address, in the PDB spelling (access, const, types); each overrides a virtual
    // its base chain declares (clang override probe, made-up-name control fails): no new slot.
    // 0x0074DD60: ?IsToEvadeAllHits@CCreatureAction_UndeadIdleUnderground@@UBE_NXZ (FableWin.pdb)
public:
    virtual bool IsToEvadeAllHits() const;
#endif
};
#endif // FABLE_ENGINE_CCREATUREACTION_UNDEADIDLEUNDERGROUND_H
