// Leaf: ?GetStateGroupType@CAIStateGroupBase@@UBE?AW4EAIStateGroupType@@XZ
// Switch route (NF318, 2026-11-23): defined on the real class of engine/CAIStateGroupBase.h under its class-model switch FABLE_RETAIL_CAISTATEGROUPBASE (was the flattened
//   free stand-in ?CAIStateGroupBase_GetStateGroupType@@YIHXZ); the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member here.
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "engine/CAIStateGroupBase.h"   // switch route: the real class model
#pragma optimize("s",on)
EAIStateGroupType CAIStateGroupBase::GetStateGroupType() const
{
    return (EAIStateGroupType)3;   // the PDB enum (enumerator names not in the repo yet; NF307 D3 literal-as-enum form)
}