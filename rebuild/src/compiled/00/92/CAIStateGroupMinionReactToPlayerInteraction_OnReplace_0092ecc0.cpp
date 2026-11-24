// Clear the two byte flags at this+0x2c / this+0x2d. __fastcall this=ecx.
// Leaf: ?OnReplace@CAIStateGroup_MinionReactToPlayerInteraction@@UAEXXZ
// Class-model move (NF322, 2026-11-23): defined on the real class of engine/CAIStateGroup_MinionReactToPlayerInteraction.h under FABLE_RETAIL_CAISTATEGROUP_MINIONREACTTOPLAYERINTERACTION_VIRTUAL_MODEL (the opt-in polymorphic model;
//   was CAIStateGroup_MinionReactToPlayerInteraction_Methods::OnReplace); the retail RTTI vftable placement names this PDB member here.
#define FABLE_RETAIL_CAISTATEGROUP_MINIONREACTTOPLAYERINTERACTION_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "engine/CAIStateGroup_MinionReactToPlayerInteraction.h"  // retyped onto the PDB layout; byte parity re-verified
#pragma pack(push,1)
#pragma pack(pop)
void CAIStateGroup_MinionReactToPlayerInteraction::OnReplace() { this->WaitingForConfirmation = false; this->DidTraderInteraction = false; }
