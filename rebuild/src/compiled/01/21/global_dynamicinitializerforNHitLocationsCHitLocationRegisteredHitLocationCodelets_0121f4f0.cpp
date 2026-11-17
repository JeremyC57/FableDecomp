// dynamic initializer for 'NHitLocations::CHitLocation::RegisteredHitLocationCodelets' (0x013BAF2C) -- construct the private static STLport map (0x18-byte header node from malloc, empty, self-linked) and register its atexit destructor 0x01229F30
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) @ 0x0121F4F0, 60 bytes.
// agentNF69 (q10821 / q10846): the static map is NHitLocations::CHitLocation::RegisteredHitLocationCodelets, a private static
//   map<EHitLocationCodelet, void (__fastcall*)(CThing&, const CHitParameters&)> (Ego_d publics + LF_STMEMBER; Ego_r.pdb 0x01011E00 /
//   FableWin.pdb 0x03FF3730 `dynamic initializer for 'NHitLocations::CHitLocation::RegisteredHitLocationCodelets'`;
//   docs/journal/2026-11/EGOD_FINDINGS.md section 1), declared by engine/NHitLocations_CHitLocation.h (FABLE_RETAIL_HITLOCATIONS_BOUND)
//   and defined here on stlport/fable_map.h, as the landed CTCAIScratchpad::Gossip pair (0x0121F160 / 0x01229CA0) does: VC7.1 emits its
//   dynamic initializer $E1 (this row: the inline map constructor, then atexit($E2)) and the atexit destructor $E2 (0x01229F30) from
//   this one definition. Was the CRetailTree_node0x18 g_tree_013baf2c stand-in (_static_init).
#define FABLE_RETAIL_HITLOCATIONS_BOUND
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_C3DVECTOR
#include "engine/CThing.h"
#include "engine/NHitLocations_CHitLocation.h"

std::map<EHitLocationCodelet, void (__fastcall*)(CThing&, CHitParameters const&)> NHitLocations::CHitLocation::RegisteredHitLocationCodelets;   // 0x013BAF2C
