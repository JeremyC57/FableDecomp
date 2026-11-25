// Leaf: ?GetStrikeEventHelperName@CCreatureActionBase@@UBE?AVCCharString@@XZ
// Switch route (NF318, 2026-11-23): defined on the real class of engine/CCreatureActionBase.h (class-model switch FABLE_RETAIL_CCREATUREACTIONBASE; was ?GetStrikeEventHelperName@CCreatureActionBase_Methods@@QAE?AVCCharString@@XZ);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address. Bytes unchanged.
// Preamble: the define / include block of the landed precedent CCreatureActionBase_OnAnimationLoaded_00692830.cpp.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "engine/CCreatureActionBase.h"
#include "engine/NCreatureActionEvents.h"

CCharString CCreatureActionBase::GetStrikeEventHelperName() const
{
    return CCharString("", -1);
}