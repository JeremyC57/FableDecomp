#ifndef FABLE_ENGINE_CCREATUREACTION_ACTIVATEZTARGET_H
#define FABLE_ENGINE_CCREATUREACTION_ACTIVATEZTARGET_H
// NF323 (2026-11-24, NF320 Q1 / NF322 recipe): the polymorphic retail class for the Clone TU, RTTI .?AVCCreatureAction_ActivateZTarget@@ : CCreatureActionBase
// (rtti_bases.tsv), no own members (retail sizeof 0xA8: the Clone allocates 0xa8 and copies only the base). Declarations only. Each
// virtual overrides the CCreatureActionBase virtual at the same slot of retail vftable 0x012738EC (RTTI-port label + the PDB name sets):
//   slot 3 = 0x008424A0 ?PerformAction@CCreatureAction_ActivateZTarget@@UAEXXZ (FableWin.pdb, Ego_r.pdb)
//   slot 11 = 0x0084C010 ?Clone@CCreatureAction_ActivateZTarget@@UBEPAVCCreatureActionBase@@XZ (FableWin.pdb, Ego_r.pdb)
// New header: no class model existed in rebuild/include (NF323). Needs FABLE_RETAIL_CCREATUREACTIONBASE (the retail base).
#include "rebuild_abi.h"
#include "engine/CCreatureActionBase.h"
class CCreatureAction_ActivateZTarget : public CCreatureActionBase {
public:
    virtual void PerformAction();
    virtual CCreatureActionBase * Clone() const;
};
FABLE_STATIC_ASSERT(sizeof(CCreatureAction_ActivateZTarget) == 0xa8);
#endif // FABLE_ENGINE_CCREATUREACTION_ACTIVATEZTARGET_H
