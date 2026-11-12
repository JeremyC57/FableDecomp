// dynamic initializer for CCharString `anonymous namespace'::TEXT_TO_SAY @ 0x013BB690 -- constructs it from the literal, then registers its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01221980, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push -1 ; push 0x1291764 ; mov ecx, 0x13bb690 ; call 0x99ebf0 ; push 0x122a2d0 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CCharString
// living at 0x013BB690: it constructs it from the literal and registers the
// destructor thunk @ 0x0122A2D0 (`mov ecx,offset object ; jmp CCharString::~CCharString`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is its PDB name
// (see Names below). One of 163 bodies of this shape.
#include "fable_string.h"      // CCharString: ctor @ 0x0099EBF0 / default @ 0x0099AED0, ~CCharString @ 0x0099EAE0

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for '`anonymous namespace'::TEXT_TO_SAY'' (0x010173B0) + FableWin.pdb
//   (0x04005910); Ego_r runs FAILURE_STRING -> TEXT_TO_SAY -> REACTION_MANAGER_DEF_INSTANTIATION_NAME x2 = retail 0x01221960 ->
//   this ("TEXT_AI_WIFE_GIFT_GIVE") -> 0x012219A0. Anniv360 Test PDB (third source): S_LDATA32 CCharString TEXT_TO_SAY
//   (internal linkage). Was g_CCharString_013bb690.
namespace {
CCharString TEXT_TO_SAY("TEXT_AI_WIFE_GIFT_GIVE", -1);    // 0x013BB690
}
