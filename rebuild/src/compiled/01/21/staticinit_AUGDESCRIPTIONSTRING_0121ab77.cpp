// dynamic initializer for const CCharString `anonymous namespace'::AUG_DESCRIPTION_STRING @ 0x013B8BB0 -- constructs it from the literal, then registers its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121AB77, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push -1 ; push 0x12546e4 ; mov ecx, 0x13b8bb0 ; call 0x99ebf0 ; push 0x1228fed ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CCharString
// living at 0x013B8BB0: it constructs it from the literal and registers the
// destructor thunk @ 0x01228FED (`mov ecx,offset object ; jmp CCharString::~CCharString`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is its PDB name
// (see Names below). One of 46 bodies of this shape.
#include "fable_string.h"      // CCharString: ctor @ 0x0099EBF0 / default @ 0x0099AED0, ~CCharString @ 0x0099EAE0

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for '`anonymous namespace'::AUG_DESCRIPTION_STRING'' (0x0100E378) +
//   FableWin.pdb (0x03FF29FE); CTCInventoryWeapons TU: both PDBs run NEWLINE, WEAPON_DESCRIPTION_STRING,
//   AUG_DESCRIPTION_STRING, WEAPON_TITLE_STRING, AUGMENTATIONS_LEFT_DESCRIPTION_STRING_M/_R, AUGMENTATIONS_MENU_ENTRY_NAME =
//   retail 0x0121AB3F (landed NEWLINE) .. 0x0121ABEB; the landed binders 0x005C574E / 0x005C5F11 / 0x005CACB9 spell these
//   const. Anniv360 Test PDB (third source): S_LDATA32 CCharString AUG_DESCRIPTION_STRING (internal linkage). Was
//   g_CCharString_013b8bb0.
namespace {
const CCharString AUG_DESCRIPTION_STRING("Augmentation_Descriptions", -1);    // 0x013B8BB0
}
