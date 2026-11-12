// dynamic initializer for CCharString `anonymous namespace'::SIT_ON_BENCH_ANIM_NAME @ 0x013BB6E4 -- constructs it from the literal, then registers its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01221AE0, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push -1 ; push 0x1292404 ; mov ecx, 0x13bb6e4 ; call 0x99ebf0 ; push 0x122a390 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CCharString
// living at 0x013BB6E4: it constructs it from the literal and registers the
// destructor thunk @ 0x0122A390 (`mov ecx,offset object ; jmp CCharString::~CCharString`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is its PDB name
// (see Names below). One of 163 bodies of this shape.
#include "fable_string.h"      // CCharString: ctor @ 0x0099EBF0 / default @ 0x0099AED0, ~CCharString @ 0x0099EAE0

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for '`anonymous namespace'::SIT_ON_BENCH_ANIM_NAME'' (0x01017510) +
//   FableWin.pdb (0x040000B0); Ego_r runs
//   SIT_ON_CHAIR/GET_UP_FROM_CHAIR/SIT_ON_BENCH/GET_UP_FROM_BENCH/SIT_ON_FLOOR/GET_UP_FROM_FLOOR_ANIM_NAME, MIN_TIME_TO_SIT,
//   RANGE_TIME_TO_SIT, MIN_/RANGE_TIME_TO_LEAVE_BETWEEN_SITTINGS = retail 0x01221AA0 .. 0x01221BA0 (MIN_TIME_TO_SIT landed at
//   0x01221B60; FPS*120, *10, *50). Anniv360 Test PDB (third source): S_LDATA32 CCharString SIT_ON_BENCH_ANIM_NAME (internal
//   linkage). Was g_CCharString_013bb6e4.
namespace {
CCharString SIT_ON_BENCH_ANIM_NAME("ST_SIT_ON_BENCH", -1);    // 0x013BB6E4
}
