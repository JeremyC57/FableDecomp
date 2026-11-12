// dynamic initializer for CCharString `anonymous namespace'::CLOCKWISE_90 @ 0x013BAD5C -- constructs it from the literal, then registers its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121ED90, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push -1 ; push 0x12697cc ; mov ecx, 0x13bad5c ; call 0x99ebf0 ; push 0x1229a40 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CCharString
// living at 0x013BAD5C: it constructs it from the literal and registers the
// destructor thunk @ 0x01229A40 (`mov ecx,offset object ; jmp CCharString::~CCharString`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is its PDB name
// (see Names below). One of 163 bodies of this shape.
#include "fable_string.h"      // CCharString: ctor @ 0x0099EBF0 / default @ 0x0099AED0, ~CCharString @ 0x0099EAE0

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for '`anonymous namespace'::CLOCKWISE_90'' (0x01011740) + FableWin.pdb
//   (0x03FF3E50); both PDBs run CLOCKWISE_90, ANTI_CLOCKWISE_90, CLOCKWISE_180, ANTI_CLOCKWISE_180, COS_45,
//   CTCShapeManager::RegisteredInstantiations = retail 0x0121ED90 .. 0x0121EE20 (TURNING_C90 .. TURNING_AC180, then cos of the
//   pi/4 constant 0x01261E50). Anniv360 Test PDB (third source): S_LDATA32 CCharString CLOCKWISE_90 (internal linkage). Was
//   g_CCharString_013bad5c.
namespace {
CCharString CLOCKWISE_90("TURNING_C90", -1);    // 0x013BAD5C
}
