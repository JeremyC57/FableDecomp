// dynamic initializer for const CCharString `anonymous namespace'::DRESSING_SIGN_POST_DEF_NAME @ 0x013BAAFC -- constructs it from the literal, then registers its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121E0E0, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push -1 ; push 0x125d898 ; mov ecx, 0x13baafc ; call 0x99ebf0 ; push 0x12293b0 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CCharString
// living at 0x013BAAFC: it constructs it from the literal and registers the
// destructor thunk @ 0x012293B0 (`mov ecx,offset object ; jmp CCharString::~CCharString`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is its PDB name
// (see Names below). One of 163 bodies of this shape.
#include "fable_string.h"      // CCharString: ctor @ 0x0099EBF0 / default @ 0x0099AED0, ~CCharString @ 0x0099EAE0

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for '`anonymous namespace'::DRESSING_SIGN_POST_DEF_NAME'' (0x01010BA0) +
//   FableWin.pdb (0x03FF3640); both PDBs run MAIN_SIGN_POST_DEF_NAME, DRESSING_SIGN_POST_DEF_NAME,
//   MAIN_OWNED_SIGN_POST_DEF_NAME, MAIN_RENT_SIGN_POST_DEF_NAME = retail 0x0121E0C0 .. 0x0121E120
//   (OBJECT_BUYABLE_HOUSE_{MAIN,DRESSING,OWNED,RENT}_SIGNPOST); landed binders spell them const. Anniv360 Test PDB (third
//   source): S_LDATA32 CCharString DRESSING_SIGN_POST_DEF_NAME (internal linkage). Was g_CCharString_013baafc.
namespace {
const CCharString DRESSING_SIGN_POST_DEF_NAME("OBJECT_BUYABLE_HOUSE_DRESSING_SIGNPOST", -1);    // 0x013BAAFC
}
