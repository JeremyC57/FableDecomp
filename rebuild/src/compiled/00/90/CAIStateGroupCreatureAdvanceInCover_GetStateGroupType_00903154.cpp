// Leaf: ?GetStateGroupType@CAIStateGroup_CreatureAdvanceInCover@@UBE?AW4EAIStateGroupType@@XZ
// Header route (NF319, 2026-11-23): defined on the real class of engine/CAIStateGroup_CreatureAdvanceInCover.h (was ?GetStateGroupType@CAIStateGroup_CreatureAdvanceInCover_Methods@@QAEHXZ); the retail RTTI vftable
//   placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address; declared by the add-only opt-in
//   FABLE_RETAIL_CAISTATEGROUP_CREATUREADVANCEINCOVER_RTTI_DECLS. Bytes unchanged.
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREADVANCEINCOVER_ISRUNNABLELEAF
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREADVANCEINCOVER_RTTI_DECLS
#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
#include "engine/CThingCreatureBase.h"
#include "engine/CAIStateGroup_CreatureAdvanceInCover.h"  // retyped onto the PDB layout; byte parity re-verified
EAIStateGroupType CAIStateGroup_CreatureAdvanceInCover::GetStateGroupType() const { return (EAIStateGroupType)1; }
