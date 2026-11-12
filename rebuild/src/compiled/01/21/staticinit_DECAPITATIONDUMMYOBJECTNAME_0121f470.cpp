// dynamic initializer for CCharString `anonymous namespace'::DECAPITATION_DUMMY_OBJECT_NAME @ 0x013BAF0C -- constructs it from the literal, then registers its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121F470, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push -1 ; push 0x12718e4 ; mov ecx, 0x13baf0c ; call 0x99ebf0 ; push 0x1229ee0 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CCharString
// living at 0x013BAF0C: it constructs it from the literal and registers the
// destructor thunk @ 0x01229EE0 (`mov ecx,offset object ; jmp CCharString::~CCharString`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is its PDB name
// (see Names below). One of 163 bodies of this shape.
#include "fable_string.h"      // CCharString: ctor @ 0x0099EBF0 / default @ 0x0099AED0, ~CCharString @ 0x0099EAE0

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for '`anonymous namespace'::DECAPITATION_DUMMY_OBJECT_NAME'' (0x01011D40)
//   + FableWin.pdb (0x03FF6130); both PDBs run DECAPITATION_DUMMY_OBJECT_NAME -> CTCAISummon::AllSummonedCreatures = retail
//   this ("DECAPITATION_POINT") -> 0x0121F490 (list). Anniv360 Test PDB (third source): S_LDATA32 CCharString
//   DECAPITATION_DUMMY_OBJECT_NAME (internal linkage). Was g_CCharString_013baf0c.
namespace {
CCharString DECAPITATION_DUMMY_OBJECT_NAME("DECAPITATION_POINT", -1);    // 0x013BAF0C
}
