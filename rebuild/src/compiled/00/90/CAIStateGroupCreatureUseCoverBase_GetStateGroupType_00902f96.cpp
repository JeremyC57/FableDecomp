// Leaf: ?GetStateGroupType@CAIStateGroup_CreatureUseCoverBase@@MBE?AW4EAIStateGroupType@@XZ
// Header route (NF319, 2026-11-23): defined on the real class of engine/CAIStateGroup_CreatureUseCoverBase.h (was ?GetStateGroupType@CAIStateGroup_CreatureUseCoverBase_Methods@@QAEHXZ); the retail RTTI vftable
//   placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address; declared by the add-only opt-in
//   FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE_RTTI_DECLS. Bytes unchanged.
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE_RTTI_DECLS
#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
class CThing;
class CThingCreatureBase;
#include "engine/CAIStateGroup_CreatureUseCoverBase.h"  // retyped onto the PDB layout; byte parity re-verified
EAIStateGroupType CAIStateGroup_CreatureUseCoverBase::GetStateGroupType() const { return (EAIStateGroupType)1; }
