// dynamic initializer for CCharString `anonymous namespace'::HEED_STRING @ 0x013BB6C4 -- constructs it from the literal, then registers its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x012219E0, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push -1 ; push 0x1291ea0 ; mov ecx, 0x13bb6c4 ; call 0x99ebf0 ; push 0x122a310 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CCharString
// living at 0x013BB6C4: it constructs it from the literal and registers the
// destructor thunk @ 0x0122A310 (`mov ecx,offset object ; jmp CCharString::~CCharString`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is its PDB name
// (see Names below). One of 163 bodies of this shape.
#include "fable_string.h"      // CCharString: ctor @ 0x0099EBF0 / default @ 0x0099AED0, ~CCharString @ 0x0099EAE0

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for '`anonymous namespace'::HEED_STRING'' (0x01017410) + FableWin.pdb
//   (0x03FFFEA0); Ego_r runs REACTION_MANAGER_DEF_INSTANTIATION_NAME x2, HEED_STRING, IGNORE_STRING, RUN_AWAY_STRING,
//   SEEN_MINOR/MODERATE/SERIOUS_CRIME_STRING = retail 0x012219A0 .. 0x01221A80 (TEXT_AI_GUARD_HEED_WARNING ..). Anniv360 Test
//   PDB (third source): S_LDATA32 CCharString HEED_STRING (internal linkage). Was g_CCharString_013bb6c4.
namespace {
CCharString HEED_STRING("TEXT_AI_GUARD_HEED_WARNING", -1);    // 0x013BB6C4
}
