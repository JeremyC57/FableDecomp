#ifndef FABLE_ENGINE_CCREATUREACTION_CALLSPOUSEHOME_H
#define FABLE_ENGINE_CCREATUREACTION_CALLSPOUSEHOME_H
// NF323 (2026-11-24, NF320 Q1 / NF322 recipe): the polymorphic retail class for the Clone TU, RTTI .?AVCCreatureAction_CallSpouseHome@@ : CCreatureActionBase
// (rtti_bases.tsv), no own members (retail sizeof 0xA8: the Clone allocates 0xa8 and copies only the base). Declarations only. Each
// virtual overrides the CCreatureActionBase virtual at the same slot of retail vftable 0x0129203C (RTTI-port label + the PDB name sets):
//   slot 3 = 0x00849140 ?PerformAction@CCreatureAction_CallSpouseHome@@UAEXXZ (FableWin.pdb, Ego_r.pdb)
//   slot 11 = 0x00933D50 ?Clone@CCreatureAction_CallSpouseHome@@UBEPAVCCreatureActionBase@@XZ (FableWin.pdb, Ego_r.pdb)
// New header: no class model existed in rebuild/include (NF323). Needs FABLE_RETAIL_CCREATUREACTIONBASE (the retail base).
#include "rebuild_abi.h"
#include "engine/CCreatureActionBase.h"
class CCreatureAction_CallSpouseHome : public CCreatureActionBase {
public:
    virtual void PerformAction();
    virtual CCreatureActionBase * Clone() const;
};
FABLE_STATIC_ASSERT(sizeof(CCreatureAction_CallSpouseHome) == 0xa8);
#endif // FABLE_ENGINE_CCREATUREACTION_CALLSPOUSEHOME_H
