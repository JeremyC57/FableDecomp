// Leaf: ?GetAnimDelayTime@CCreatureActionBase@@UBEJXZ
// CCreatureActionBase::GetAnimDelayTime @ 0x00692460
// Retail: 33 c0 c3  ->  xor eax,eax ; ret
// Virtual const this-only accessor returning long 0.
// Identity: retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names ?GetAnimDelayTime@CCreatureActionBase@@UBEJXZ at this
//   address; the PDB member is public virtual const (Ego_r / FableWin).
// Switch route (NF318, 2026-11-23): the class model is engine/CCreatureActionBase.h under FABLE_RETAIL_CCREATUREACTIONBASE (was the flattened
//   free stand-in ?CCreatureActionBase_GetAnimDelayTime@@YIJPBVCCreatureActionBase@@@Z); preamble from the landed precedent
//   CCreatureActionBase_RegisterActionEventTypes_006932b0.cpp. Bytes unchanged.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "engine/CCreatureActionBase.h"
#include "engine/NCreatureActionEvents.h"

long CCreatureActionBase::GetAnimDelayTime() const
{
    return 0;
}
