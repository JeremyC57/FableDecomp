// Leaf: ?GetStateGroupType@CAIStateGroup_HoverAttack@@UBE?AW4EAIStateGroupType@@XZ
// Class-model move (NF322, 2026-11-23): defined on the real class of engine/CAIStateGroup_HoverAttack.h under FABLE_RETAIL_CAISTATEGROUP_HOVERATTACK_VIRTUAL_MODEL (the opt-in polymorphic model;
//   was CAIStateGroup_HoverAttack_Methods::GetStateGroupType); the retail RTTI vftable placement names this PDB member here.
#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
#define FABLE_RETAIL_CAISTATEGROUP_HOVERATTACK_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE_VSLOTS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUP_HOVERMOVEMENTBASE_MOVETOPOSANDFACEPOS
#include "engine/CAIStateGroup_HoverAttack.h"  // retyped onto the PDB layout; byte parity re-verified
EAIStateGroupType CAIStateGroup_HoverAttack::GetStateGroupType() const { return (EAIStateGroupType)1; }
