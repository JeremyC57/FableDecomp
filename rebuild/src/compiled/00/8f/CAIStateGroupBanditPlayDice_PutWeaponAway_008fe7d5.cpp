// Constant-return member: returns true. __fastcall this=ecx.
// Leaf: ?PutWeaponAway@CAIStateGroup_BanditPlayDice@@MAE_NXZ
// Header route (NF319, 2026-11-23): defined on the real class of engine/CAIStateGroup_BanditPlayDice.h (was ?PutWeaponAway@CAIStateGroup_BanditPlayDice_Methods@@QAE_NXZ); the retail RTTI vftable
//   placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address;
//   declared there under FABLE_RETAIL_CAISTATEGROUP_BANDITPLAYDICE + FABLE_RETAIL_CAISTATEGROUP_BANDITPLAYDICE_VIRTUALS (switches landed rows define). Bytes unchanged.
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_BANDITPLAYDICE
#define FABLE_RETAIL_CAISTATEGROUP_BANDITPLAYDICE_VIRTUALS
#include "engine/CAIStateGroup_BanditPlayDice.h"  // retyped onto the PDB layout; byte parity re-verified
bool CAIStateGroup_BanditPlayDice::PutWeaponAway() { return true; }
