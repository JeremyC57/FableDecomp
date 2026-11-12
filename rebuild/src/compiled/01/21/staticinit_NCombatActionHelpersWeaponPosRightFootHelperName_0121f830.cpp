// dynamic initializer for static CCharString NCombatActionHelpers::WeaponPosRightFootHelperName @ 0x013BAFF8 -- constructs it from the literal, then registers its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121F830, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push -1 ; push 0x1288c08 ; mov ecx, 0x13baff8 ; call 0x99ebf0 ; push 0x122a1f0 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CCharString
// living at 0x013BAFF8: it constructs it from the literal and registers the
// destructor thunk @ 0x0122A1F0 (`mov ecx,offset object ; jmp CCharString::~CCharString`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is its PDB name
// (see Names below). One of 163 bodies of this shape.
#include "fable_string.h"      // CCharString: ctor @ 0x0099EBF0 / default @ 0x0099AED0, ~CCharString @ 0x0099EAE0

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for 'NCombatActionHelpers::WeaponPosRightFootHelperName'' (0x01013670) +
//   FableWin.pdb (0x03FFAA20); both PDBs name NCombatActionHelpers::WeaponPosLeftFootHelperName / WeaponPosRightFootHelperName
//   = retail 0x0121F810 ("weapon_pos_left_foot"), 0x0121F830 ("weapon_pos_right_foot"); Anniv360 Test PDB (third source):
//   S_LDATA32 CCharString (internal linkage). Was g_CCharString_013baff8.
namespace NCombatActionHelpers {
static CCharString WeaponPosRightFootHelperName("weapon_pos_right_foot", -1);    // 0x013BAFF8 (internal linkage)
}
