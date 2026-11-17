// NHitLocations::CHitLocation::ProcessHitLocationCode(CThing& thing, const CHitParameters& hit_parameters) -- look the def's codelet id up in the static RegisteredHitLocationCodelets map and, when registered, call the codelet with the thing and the hit parameters
// Leaf: ?ProcessHitLocationCode@CHitLocation@NHitLocations@@QAEXAAVCThing@@ABVCHitParameters@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00815E80, length 49 bytes; thiscall, `ret 8`.
// Name: Ego_r.pdb + FableWin.pdb carry the Leaf symbol (Ego_d: ProcessHitLocationCode(CThing&, const CHitParameters&), the codelet signature); engine/NHitLocations_CHitLocation.h
//   FABLE_RETAIL_HITLOCATIONS_BOUND declares it. agentNF69 (q10821): the map 0x013BAF2C is the private static
//   NHitLocations::CHitLocation::RegisteredHitLocationCodelets (Ego_d publics + LF_STMEMBER; docs/journal/2026-11/EGOD_FINDINGS.md
//   section 1), map<EHitLocationCodelet, void (__fastcall*)(CThing&, const CHitParameters&)>; was the CRetailTree_longkey
//   g_hitLocationCodes_013baf2c stand-in with a local CHitLocation model and long arguments.
// Body: PDef->HitLocationCodelet (CHitLocationDef +0x78, retail; Ego_r +0x7C over a 16-byte Dinkumware vector) is the key; STLport's map::find
//   calls the out-of-line member-template _Rb_tree::find 0x00817740 (hidden iterator result, key by reference), the node is compared with
//   end() (the header, `cmp eax,[13BAF2Ch]`) and the codelet (+0x14) is called `__fastcall` (ecx = thing, edx = hit_parameters).
#define FABLE_RETAIL_HITLOCATIONS_BOUND
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CHITLOCATION_TREGISTEREDHITLOCATIONCODELETMAP
#define FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE_DECL
#include "engine/CThing.h"
#include "engine/NHitLocations_CHitLocation.h"

void NHitLocations::CHitLocation::ProcessHitLocationCode(CThing& thing, const CHitParameters& hit_parameters)
{
    TRegisteredHitLocationCodeletMap::iterator it = RegisteredHitLocationCodelets.find(PDef->HitLocationCodelet);
    if (it != RegisteredHitLocationCodelets.end())
        (*it).second(thing, hit_parameters);
}
