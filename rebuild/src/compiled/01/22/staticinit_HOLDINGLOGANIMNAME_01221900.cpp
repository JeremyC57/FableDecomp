// dynamic initializer for CCharString `anonymous namespace'::HOLDING_LOG_ANIM_NAME @ 0x013BB67C -- constructs it from the literal, then registers its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01221900, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push -1 ; push 0x128fd58 ; mov ecx, 0x13bb67c ; call 0x99ebf0 ; push 0x122a290 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CCharString
// living at 0x013BB67C: it constructs it from the literal and registers the
// destructor thunk @ 0x0122A290 (`mov ecx,offset object ; jmp CCharString::~CCharString`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is its PDB name
// (see Names below). One of 163 bodies of this shape.
#include "fable_string.h"      // CCharString: ctor @ 0x0099EBF0 / default @ 0x0099AED0, ~CCharString @ 0x0099EAE0

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for '`anonymous namespace'::HOLDING_LOG_ANIM_NAME'' (0x01017330) +
//   FableWin.pdb (0x04000420); Ego_r runs REACTION_MANAGER_DEF_INSTANTIATION_NAME, SIT_ON_FLOOR_ANIM_NAME,
//   GET_UP_FROM_FLOOR_ANIM_NAME, LOG_DEF_NAME, PICK_UP_ANIM_NAME, HOLDING_LOG_ANIM_NAME, PUT_LOG_IN_BONFIRE_ANIM_NAME = retail
//   0x01221860 .. 0x01221920 (ST_SIT_ON_FLOOR .. ST_WORK_PUT_CRATE). Anniv360 Test PDB (third source): S_LDATA32 CCharString
//   HOLDING_LOG_ANIM_NAME (internal linkage). Was g_CCharString_013bb67c.
namespace {
CCharString HOLDING_LOG_ANIM_NAME("ST_HOLDING_CRATE", -1);    // 0x013BB67C
}
