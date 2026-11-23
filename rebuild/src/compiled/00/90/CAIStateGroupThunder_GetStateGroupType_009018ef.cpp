// Leaf: ?GetStateGroupType@CAIStateGroup_Thunder@@UBE?AW4EAIStateGroupType@@XZ
// Header route (NF319, 2026-11-23): defined on the real class of engine/CAIStateGroup_Thunder.h (was ?GetStateGroupType@CAIStateGroup_Thunder_Methods@@QAEHXZ); the retail RTTI vftable
//   placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address; declared by the add-only opt-in
//   FABLE_RETAIL_CAISTATEGROUP_THUNDER_RTTI_DECLS. Bytes unchanged.
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_THUNDER
#define FABLE_RETAIL_CAISTATEGROUP_THUNDER_RTTI_DECLS
#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
#include "engine/CAIStateGroup_Thunder.h"  // retyped onto the PDB layout; byte parity re-verified
EAIStateGroupType CAIStateGroup_Thunder::GetStateGroupType() const { return (EAIStateGroupType)1; }
