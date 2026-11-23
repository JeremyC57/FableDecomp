// Leaf: ?GetStateGroupType@CAIStateGroup_BriarRoseWillUse@@UBE?AW4EAIStateGroupType@@XZ
// Header route (NF319, 2026-11-23): defined on the real class of engine/CAIStateGroup_BriarRoseWillUse.h (was ?GetStateGroupType@CAIStateGroup_BriarRoseWillUse_Methods@@QAEHXZ); the retail RTTI vftable
//   placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address; declared by the add-only opt-in
//   FABLE_RETAIL_CAISTATEGROUP_BRIARROSEWILLUSE_RTTI_DECLS. Bytes unchanged.
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_BRIARROSEWILLUSE
#define FABLE_RETAIL_CAISTATEGROUP_BRIARROSEWILLUSE_RTTI_DECLS
#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CAIStateGroup_BriarRoseWillUse.h"  // retyped onto the PDB layout; byte parity re-verified
EAIStateGroupType CAIStateGroup_BriarRoseWillUse::GetStateGroupType() const { return (EAIStateGroupType)1; }
