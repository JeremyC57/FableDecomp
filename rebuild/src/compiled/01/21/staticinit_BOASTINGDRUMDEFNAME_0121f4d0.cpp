// dynamic initializer for CCharString `anonymous namespace'::BOASTING_DRUM_DEF_NAME @ 0x013BAF28 -- constructs it from the literal, then registers its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121F4D0, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push -1 ; push 0x127246c ; mov ecx, 0x13baf28 ; call 0x99ebf0 ; push 0x1229f20 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CCharString
// living at 0x013BAF28: it constructs it from the literal and registers the
// destructor thunk @ 0x01229F20 (`mov ecx,offset object ; jmp CCharString::~CCharString`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is its PDB name
// (see Names below). One of 163 bodies of this shape.
#include "fable_string.h"      // CCharString: ctor @ 0x0099EBF0 / default @ 0x0099AED0, ~CCharString @ 0x0099EAE0

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for '`anonymous namespace'::BOASTING_DRUM_DEF_NAME'' (0x01011DE0) +
//   FableWin.pdb (0x03FF45B0); Ego_r runs BOASTING_POSITION_NAME_PREFIX, BOASTING_DRUM_DEF_NAME,
//   NHitLocations::CHitLocation::RegisteredHitLocationCodelets = retail 0x0121F4B0 ("BOASTING_DRUM_POINT_"), 0x0121F4D0
//   ("OBJECT_HERO_BOASTING_PODIUM_DRUM_01"), 0x0121F4F0 (tree). Anniv360 Test PDB (third source): S_LDATA32 CCharString
//   BOASTING_DRUM_DEF_NAME (internal linkage). Was g_CCharString_013baf28.
namespace {
CCharString BOASTING_DRUM_DEF_NAME("OBJECT_HERO_BOASTING_PODIUM_DRUM_01", -1);    // 0x013BAF28
}
