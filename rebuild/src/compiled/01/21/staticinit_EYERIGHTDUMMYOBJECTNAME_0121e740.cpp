// dynamic initializer for const CCharString EYE_RIGHT_DUMMY_OBJECT_NAME @ 0x013BAC58 -- constructs it from the literal, then registers its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121E740, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push -1 ; push 0x125c28c ; mov ecx, 0x13bac58 ; call 0x99ebf0 ; push 0x12296f0 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CCharString
// living at 0x013BAC58: it constructs it from the literal and registers the
// destructor thunk @ 0x012296F0 (`mov ecx,offset object ; jmp CCharString::~CCharString`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is its PDB name
// (see Names below). One of 163 bodies of this shape.
#include "fable_string.h"      // CCharString: ctor @ 0x0099EBF0 / default @ 0x0099AED0, ~CCharString @ 0x0099EAE0

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for 'EYE_RIGHT_DUMMY_OBJECT_NAME'' (0x010110E0) + FableWin.pdb
//   (0x03FF6A00); both PDBs run the 45 helper-point names PRIMARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME ..
//   COMBAT_ACTION_EVENT_ALLOW_MOVEMENT_INTERRUPT, then ST_HEAD_TURN_H/_V (landed 0x0121EBA0/0x0121EBC0) and the 5 CARRY_SLOT_*
//   names; retail 0x0121E600 .. 0x0121EC60 is the same run one for one (WEAPON_FOCUS_01 .. ALLOW_MOVEMENT_INTERRUPT,
//   CARRY_SLOT_*). Anniv360 Test PDB (third source): ?EYE_RIGHT_DUMMY_OBJECT_NAME@@3VCCharString@@B. Was
//   g_CCharString_013bac58.
extern const CCharString EYE_RIGHT_DUMMY_OBJECT_NAME("EYE_SET_R", -1);    // 0x013BAC58 ?EYE_RIGHT_DUMMY_OBJECT_NAME@@3VCCharString@@B
