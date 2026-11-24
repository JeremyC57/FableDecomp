// Leaf: ?GetStateGroupType@CAIStateGroup_WhisperPotionCombat@@UBE?AW4EAIStateGroupType@@XZ
// Class-model move (NF322, 2026-11-23): defined on the real class of engine/CAIStateGroup_WhisperPotionCombat.h under FABLE_RETAIL_CAISTATEGROUP_WHISPERPOTIONCOMBAT_VIRTUAL_MODEL (the opt-in polymorphic model;
//   was CAIStateGroup_WhisperPotionCombat_Methods::GetStateGroupType); the retail RTTI vftable placement names this PDB member here.
#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
#define FABLE_RETAIL_CAISTATEGROUP_WHISPERPOTIONCOMBAT_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE_VSLOTS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CAIStateGroup_WhisperPotionCombat.h"  // retyped onto the PDB layout; byte parity re-verified
EAIStateGroupType CAIStateGroup_WhisperPotionCombat::GetStateGroupType() const { return (EAIStateGroupType)1; }
