// dynamic initializer for const CCharString MESH_LIGHT_POINT_NAME @ 0x013BACC8 -- constructs it from the literal, then registers its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121EAC0, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push -1 ; push 0x126539c ; mov ecx, 0x13bacc8 ; call 0x99ebf0 ; push 0x12298b0 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CCharString
// living at 0x013BACC8: it constructs it from the literal and registers the
// destructor thunk @ 0x012298B0 (`mov ecx,offset object ; jmp CCharString::~CCharString`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is its PDB name
// (see Names below). One of 163 bodies of this shape.
#include "fable_string.h"      // CCharString: ctor @ 0x0099EBF0 / default @ 0x0099AED0, ~CCharString @ 0x0099EAE0

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for 'MESH_LIGHT_POINT_NAME'' (0x01011460) + FableWin.pdb (0x03FF6F40);
//   both PDBs run the 45 helper-point names PRIMARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME ..
//   COMBAT_ACTION_EVENT_ALLOW_MOVEMENT_INTERRUPT, then ST_HEAD_TURN_H/_V (landed 0x0121EBA0/0x0121EBC0) and the 5 CARRY_SLOT_*
//   names; retail 0x0121E600 .. 0x0121EC60 is the same run one for one (WEAPON_FOCUS_01 .. ALLOW_MOVEMENT_INTERRUPT,
//   CARRY_SLOT_*). Anniv360 Test PDB (third source): ?MESH_LIGHT_POINT_NAME@@3VCCharString@@B. Was g_CCharString_013bacc8.
extern const CCharString MESH_LIGHT_POINT_NAME("LIGHT_POINT", -1);    // 0x013BACC8 ?MESH_LIGHT_POINT_NAME@@3VCCharString@@B
