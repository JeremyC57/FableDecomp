// Leaf: ?GetStateGroupType@CAIStateGroup_SeekEnemy@@UBE?AW4EAIStateGroupType@@XZ
// Class-model move (NF322, 2026-11-23): defined on the real class of engine/CAIStateGroup_SeekEnemy.h under FABLE_RETAIL_CAISTATEGROUP_SEEKENEMY_VIRTUAL_MODEL (the opt-in polymorphic model;
//   was CAIStateGroup_SeekEnemy_Methods::GetStateGroupType); the retail RTTI vftable placement names this PDB member here.
#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
#define FABLE_RETAIL_CAISTATEGROUP_SEEKENEMY_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "engine/CAIStateGroup_SeekEnemy.h"  // retyped onto the PDB layout; byte parity re-verified
EAIStateGroupType CAIStateGroup_SeekEnemy::GetStateGroupType() const { return (EAIStateGroupType)1; }
