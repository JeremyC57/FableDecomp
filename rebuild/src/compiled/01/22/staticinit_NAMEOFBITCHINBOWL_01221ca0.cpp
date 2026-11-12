// dynamic initializer for CCharString `anonymous namespace'::NAME_OF_BITCHIN_BOWL @ 0x013BB72C -- constructs it from the literal, then registers its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01221CA0, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push -1 ; push 0x1292d10 ; mov ecx, 0x13bb72c ; call 0x99ebf0 ; push 0x122a430 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CCharString
// living at 0x013BB72C: it constructs it from the literal and registers the
// destructor thunk @ 0x0122A430 (`mov ecx,offset object ; jmp CCharString::~CCharString`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is its PDB name
// (see Names below). One of 163 bodies of this shape.
#include "fable_string.h"      // CCharString: ctor @ 0x0099EBF0 / default @ 0x0099AED0, ~CCharString @ 0x0099EAE0

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for '`anonymous namespace'::NAME_OF_BITCHIN_BOWL'' (0x010176E0) +
//   FableWin.pdb (0x04000480); Ego_r runs NAME_OF_BITCHIN_BOWL, TIME_TO_LEAVE_BETWEEN_SPEAKING = retail 0x01221CA0
//   ("OBJECT_EATING_BOWL_NONSE"), 0x01221CC0 (FPS*3). Anniv360 Test PDB (third source): S_LDATA32 CCharString
//   NAME_OF_BITCHIN_BOWL (internal linkage). Was g_CCharString_013bb72c.
namespace {
CCharString NAME_OF_BITCHIN_BOWL("OBJECT_EATING_BOWL_NONSE", -1);    // 0x013BB72C
}
