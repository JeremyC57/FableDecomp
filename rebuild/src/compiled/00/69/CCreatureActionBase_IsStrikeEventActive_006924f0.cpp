// Leaf: ?IsStrikeEventActive@CCreatureActionBase@@UBE_NXZ
// CCreatureActionBase::IsStrikeEventActive @ 0x006924F0: bool IsStrikeEventActive() const -> xor al,al ; ret
// Identity: retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names ?IsStrikeEventActive@CCreatureActionBase@@UBE_NXZ at this
//   address; the PDB member is public virtual const (Ego_r / FableWin).
// Switch route (NF318, 2026-11-23): the class model is engine/CCreatureActionBase.h under FABLE_RETAIL_CCREATUREACTIONBASE (was the flattened
//   free stand-in ?CCreatureActionBase_IsStrikeEventActive@@YI_NPBVCCreatureActionBase@@@Z); preamble from the landed precedent
//   CCreatureActionBase_RegisterActionEventTypes_006932b0.cpp. Bytes unchanged.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "engine/CCreatureActionBase.h"
#include "engine/NCreatureActionEvents.h"

bool CCreatureActionBase::IsStrikeEventActive() const
{
    return false;
}
