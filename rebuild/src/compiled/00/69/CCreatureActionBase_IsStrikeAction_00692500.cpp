// Leaf: ?IsStrikeAction@CCreatureActionBase@@UBE_NXZ
// CCreatureActionBase::IsStrikeAction @ 0x00692500: bool IsStrikeAction() const -> xor al,al ; ret
// Identity: retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names ?IsStrikeAction@CCreatureActionBase@@UBE_NXZ at this
//   address; the PDB member is public virtual const (Ego_r / FableWin).
// Switch route (NF318, 2026-11-23): the class model is engine/CCreatureActionBase.h under FABLE_RETAIL_CCREATUREACTIONBASE (was the flattened
//   free stand-in ?CCreatureActionBase_IsStrikeAction@@YI_NPBVCCreatureActionBase@@@Z); preamble from the landed precedent
//   CCreatureActionBase_RegisterActionEventTypes_006932b0.cpp. Bytes unchanged.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "engine/CCreatureActionBase.h"
#include "engine/NCreatureActionEvents.h"

bool CCreatureActionBase::IsStrikeAction() const
{
    return false;
}
