// dynamic atexit destructor for 'NHitLocations::CHitLocation::RegisteredHitLocationCodelets' (0x013BAF2C) -- clear the private static STLport map (recursive _M_erase 0x00817280 from the root, header relinked, size zeroed) and free its header node
// Leaf: _$E2
// Retail: Fable.exe (TLC, VC7.1) @ 0x01229F30, 84 bytes.
// agentNF69 (q10821 / q10850): the static map is NHitLocations::CHitLocation::RegisteredHitLocationCodelets, a private static
//   map<EHitLocationCodelet, void (__fastcall*)(CThing&, const CHitParameters&)> (Ego_d publics + LF_STMEMBER; Ego_r.pdb 0x01011E00 / 0x0101CC00 and
//   FableWin.pdb 0x03FF3730 / 0x0400F920 `dynamic initializer for` / `dynamic atexit destructor for 'NHitLocations::CHitLocation::RegisteredHitLocationCodelets'`;
//   docs/journal/2026-11/EGOD_FINDINGS.md section 1), declared by engine/NHitLocations_CHitLocation.h (FABLE_RETAIL_HITLOCATIONS_BOUND)
//   and defined here on stlport/fable_map.h, as the landed CTCAIScratchpad::Gossip pair (0x0121F160 / 0x01229CA0) does: VC7.1 emits its
//   dynamic initializer $E1 (0x0121F4F0: the inline map constructor, then atexit($E2)) and the atexit destructor $E2 (this row: the inline ~map with the out-of-line _M_erase 0x00817280, then free of the header node) from
//   this one definition. Was the CRetailTree / SNode stand-in (_unattributed::Cleanup_LTextTreeWalker_1_01229f30, a propagated label).
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
