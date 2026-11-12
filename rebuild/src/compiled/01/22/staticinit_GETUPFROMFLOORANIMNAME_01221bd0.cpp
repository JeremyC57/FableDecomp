// dynamic initializer for CCharString `anonymous namespace'::GET_UP_FROM_FLOOR_ANIM_NAME @ 0x013BB708 -- constructs it from the literal, then registers its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01221BD0, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push -1 ; push 0x1278660 ; mov ecx, 0x13bb708 ; call 0x99ebf0 ; push 0x122a3e0 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CCharString
// living at 0x013BB708: it constructs it from the literal and registers the
// destructor thunk @ 0x0122A3E0 (`mov ecx,offset object ; jmp CCharString::~CCharString`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is its PDB name
// (see Names below). One of 163 bodies of this shape.
#include "fable_string.h"      // CCharString: ctor @ 0x0099EBF0 / default @ 0x0099AED0, ~CCharString @ 0x0099EAE0

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for '`anonymous namespace'::GET_UP_FROM_FLOOR_ANIM_NAME''
//   (0x010172D0/0x01017570/0x01017610) + FableWin.pdb (0x04000020/0x04000140/0x04000220); Ego_r runs SIT_ON_FLOOR_ANIM_NAME,
//   GET_UP_FROM_FLOOR_ANIM_NAME, MIN_/RANGE_TIME_TO_LEAVE_BETWEEN_SITTINGS, TIME_TO_LEAVE_SHOUT_MESSAGE = retail 0x01221BB0 ..
//   0x01221C10 (FPS*10, *50, *5). Anniv360 Test PDB (third source): S_LDATA32 CCharString GET_UP_FROM_FLOOR_ANIM_NAME (internal
//   linkage). Was g_CCharString_013bb708.
namespace {
CCharString GET_UP_FROM_FLOOR_ANIM_NAME("ST_STAND_UP_FROM_FLOOR", -1);    // 0x013BB708
}
