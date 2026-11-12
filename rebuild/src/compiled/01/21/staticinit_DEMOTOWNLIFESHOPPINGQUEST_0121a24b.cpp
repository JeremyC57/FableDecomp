// dynamic initializer for CCharString `anonymous namespace'::DEMO_TOWNLIFE_SHOPPING_QUEST @ 0x013B874C -- constructs it from the literal, then registers its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121A24B, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push -1 ; push 0x1231124 ; mov ecx, 0x13b874c ; call 0x99ebf0 ; push 0x1228abf ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CCharString
// living at 0x013B874C: it constructs it from the literal and registers the
// destructor thunk @ 0x01228ABF (`mov ecx,offset object ; jmp CCharString::~CCharString`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is its PDB name
// (see Names below). One of 46 bodies of this shape.
#include "fable_string.h"      // CCharString: ctor @ 0x0099EBF0 / default @ 0x0099AED0, ~CCharString @ 0x0099EAE0

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for '`anonymous namespace'::DEMO_TOWNLIFE_SHOPPING_QUEST'' (0x0100C65B) +
//   FableWin.pdb (0x03FDE7B5); the packed /Os demo TU: both PDBs run DEMO_WORLD_NAME, 7 DEMO_*_HOLY_SITE, 7 DEMO_*_QUEST,
//   DEFAULT_FRONT_END_FONT (Ego_r with retail's 0x1B/0x1D sizes) = retail 0x0121A11C .. 0x0121A2BF literal for literal
//   (FinalAlbion.wld, E3*HSP, Q_E3_*, ENG_ARIAL_16). Anniv360 Test PDB (third source): S_LDATA32 CCharString
//   DEMO_TOWNLIFE_SHOPPING_QUEST (internal linkage). Was g_CCharString_013b874c.
namespace {
CCharString DEMO_TOWNLIFE_SHOPPING_QUEST("Q_E3_TownShopsMarriage", -1);    // 0x013B874C
}
