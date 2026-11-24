// Leaf: ?GetStateGroupType@CAIStateGroup_CreatureTakeCover@@UBE?AW4EAIStateGroupType@@XZ
// Header route (NF319, 2026-11-23): defined on the real class of engine/CAIStateGroup_CreatureTakeCover.h (was ?GetStateGroupType@CAIStateGroup_CreatureTakeCover_Methods@@QAEHXZ); the retail RTTI vftable
//   placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address; declared by the add-only opt-in
//   FABLE_RETAIL_CAISTATEGROUP_CREATURETAKECOVER_RTTI_DECLS. Bytes unchanged.
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_SPEECH
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE
#define FABLE_RETAIL_CAISTATEGROUP_CREATURETAKECOVER_ISRUNNABLELEAF
#define FABLE_RETAIL_CAISTATEGROUP_CREATURETAKECOVER_RTTI_DECLS
#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
#include "engine/CIntelligentPointer.h"
#include "engine/CWorld.h"
#include "engine/CAIStateGroup_CreatureTakeCover.h"  // retyped onto the PDB layout; byte parity re-verified
EAIStateGroupType CAIStateGroup_CreatureTakeCover::GetStateGroupType() const { return (EAIStateGroupType)1; }
