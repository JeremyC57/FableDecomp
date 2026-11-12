// dynamic initializer for CCharString `anonymous namespace'::DEAD_ANIM_NAME @ 0x013BAF40 -- constructs it from the literal, then registers its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121F570, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push -1 ; push 0x1272af0 ; mov ecx, 0x13baf40 ; call 0x99ebf0 ; push 0x1229fb0 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CCharString
// living at 0x013BAF40: it constructs it from the literal and registers the
// destructor thunk @ 0x01229FB0 (`mov ecx,offset object ; jmp CCharString::~CCharString`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is its PDB name
// (see Names below). One of 163 bodies of this shape.
#include "fable_string.h"      // CCharString: ctor @ 0x0099EBF0 / default @ 0x0099AED0, ~CCharString @ 0x0099EAE0

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for '`anonymous namespace'::DEAD_ANIM_NAME'' (0x01011F90) + FableWin.pdb
//   (0x03FF72A0); both PDBs name `anonymous namespace'::DEAD_ANIM_NAME; FableWin runs minimapFilePathArtist/Progmr ..
//   DEAD_ANIM_NAME as retail 0x0121F530 .. this; literal STANDARD_DEAD. Anniv360 Test PDB (third source): S_LDATA32 CCharString
//   DEAD_ANIM_NAME (internal linkage). Was g_CCharString_013baf40.
namespace {
CCharString DEAD_ANIM_NAME("STANDARD_DEAD", -1);    // 0x013BAF40
}
