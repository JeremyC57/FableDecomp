// dynamic initializer for CCharString `anonymous namespace'::OPEN_BRACKET @ 0x013CAA48 -- constructs it from the literal, then registers its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x012241D0, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push -1 ; push 0x122e028 ; mov ecx, 0x13caa48 ; call 0x99ebf0 ; push 0x122a8d0 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CCharString
// living at 0x013CAA48: it constructs it from the literal and registers the
// destructor thunk @ 0x0122A8D0 (`mov ecx,offset object ; jmp CCharString::~CCharString`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is its PDB name
// (see Names below). One of 163 bodies of this shape.
#include "fable_string.h"      // CCharString: ctor @ 0x0099EBF0 / default @ 0x0099AED0, ~CCharString @ 0x0099EAE0

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for '`anonymous namespace'::OPEN_BRACKET'' (0x0101AD70) + FableWin.pdb
//   (0x0400B030); both PDBs run OPEN_BRACKET, CLOSE_BRACKET, SEPARATOR = retail 0x012241D0 ("("), 0x012241F0 (")"), 0x01224210
//   (","). Anniv360 Test PDB (third source): S_LDATA32 CCharString OPEN_BRACKET (internal linkage). Was g_CCharString_013caa48.
namespace {
CCharString OPEN_BRACKET("(", -1);    // 0x013CAA48
}
