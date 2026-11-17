// NHitLocations::CHitLocation::StatRegisterClass() -- static registration: insert the four hit-location codelets (ids 1..4) into the private static map RegisteredHitLocationCodelets
// Leaf: ?StatRegisterClass@CHitLocation@NHitLocations@@SIXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008162F0, length 147 bytes (+13 int3 padding); public static __fastcall, no arguments.
// Identity (agentNF69, q10819): Ego_d publics + Ego_r.pdb / FableWin.pdb ?StatRegisterClass@CHitLocation@NHitLocations@@SIXXZ
//   (docs/journal/2026-11/EGOD_FINDINGS.md section 1); called once, from CTCBase::RegisterTCs 0x004EE286, which already binds this
//   spelling (engine/NHitLocations_CHitLocation.h FABLE_RETAIL_CHITLOCATION_STATREGISTERCLASS); Ego_d puts it in tc_hit_locations.obj
//   with the hit-location rows that are its retail neighbours. Was the made-up _global::RegisterThingHandlers.
// Body: four map::insert(value_type(id, codelet)) calls, each STLport's inline map::insert -> the out-of-line _Rb_tree::insert_unique
//   0x00818000 on RegisteredHitLocationCodelets 0x013BAF2C (q10821), pair<iterator, bool> through a hidden pointer, ignored.
//   The ids are EHitLocationCodelet values (Ego_d: 1 TRAP_DEACTIVATE, 2 AUTO_KILL, 3 STRAW_DUMMY, 4 DRAGON; the enum's enumerators are
//   in no shared header, so they are written as casts) and the codelets the four NHitLocationCodelets functions (Ego_r.pdb
//   0x0091B8E0 / 0x0091B8D0 / 0x0091B940 / 0x0091BA10, Ego_d hit_location_codelets.obj; q10833..q10836), all
//   `__fastcall (CThing&, const CHitParameters&)`. Were the CThing::HandlerTypeN member stand-ins.
// Shape: the value_type temporary stores both words after the two pushes (GOTCHAS chunk 149).
#define FABLE_RETAIL_HITLOCATIONS_BOUND
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CHITLOCATION_TREGISTEREDHITLOCATIONCODELETMAP
#include "engine/CThing.h"
#include "engine/NHitLocations_CHitLocation.h"

namespace NHitLocationCodelets {
void __fastcall HLCTrapDeactivate(CThing& thing, const CHitParameters& hit_parameters);   // 0x008F5D00
void __fastcall HLCAutoKill(CThing& thing, const CHitParameters& hit_parameters);         // 0x008F5CF0
void __fastcall HLCStrawDummy(CThing& thing, const CHitParameters& hit_parameters);       // 0x008F5D40
void __fastcall HLCDragon(CThing& thing, const CHitParameters& hit_parameters);           // 0x008F5E00
}

void NHitLocations::CHitLocation::StatRegisterClass()
{
    RegisteredHitLocationCodelets.insert(TRegisteredHitLocationCodeletMap::value_type((EHitLocationCodelet)1, &NHitLocationCodelets::HLCTrapDeactivate));
    RegisteredHitLocationCodelets.insert(TRegisteredHitLocationCodeletMap::value_type((EHitLocationCodelet)2, &NHitLocationCodelets::HLCAutoKill));
    RegisteredHitLocationCodelets.insert(TRegisteredHitLocationCodeletMap::value_type((EHitLocationCodelet)3, &NHitLocationCodelets::HLCStrawDummy));
    RegisteredHitLocationCodelets.insert(TRegisteredHitLocationCodeletMap::value_type((EHitLocationCodelet)4, &NHitLocationCodelets::HLCDragon));
}
