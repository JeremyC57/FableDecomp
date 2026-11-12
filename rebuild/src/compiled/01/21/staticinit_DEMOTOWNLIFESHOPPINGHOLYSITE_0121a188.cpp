// dynamic initializer for CWideString `anonymous namespace'::DEMO_TOWNLIFE_SHOPPING_HOLY_SITE @ 0x013B8730 -- constructs it from the literal, then registers its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121A188, oracle length 27 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push 0x12310ac ; mov ecx, 0x13b8730 ; call 0x99b6b0 ; push 0x1228a79 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CWideString
// living at 0x013B8730: it default-constructs it out of line and registers the
// destructor thunk @ 0x01228A79 (`mov ecx,offset object ; jmp CWideString::~CWideString`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is its PDB name
// (see Names below). One of 9 bodies of this shape.
#include "fable_string.h"      // CWideString: CWideString(const wchar_t*) @ 0x0099B6B0 (landed), ~CWideString

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for '`anonymous namespace'::DEMO_TOWNLIFE_SHOPPING_HOLY_SITE''
//   (0x0100C598) + FableWin.pdb (0x03FDE6A2); the packed /Os demo TU: both PDBs run DEMO_WORLD_NAME, 7 DEMO_*_HOLY_SITE, 7
//   DEMO_*_QUEST, DEFAULT_FRONT_END_FONT (Ego_r with retail's 0x1B/0x1D sizes) = retail 0x0121A11C .. 0x0121A2BF literal for
//   literal (FinalAlbion.wld, E3*HSP, Q_E3_*, ENG_ARIAL_16). Anniv360 Test PDB (third source): S_LDATA32 CWideString
//   DEMO_TOWNLIFE_SHOPPING_HOLY_SITE (internal linkage). Was g_CWideString_013b8730.
namespace {
CWideString DEMO_TOWNLIFE_SHOPPING_HOLY_SITE(L"E3ShopHSP");    // 0x013B8730
}
