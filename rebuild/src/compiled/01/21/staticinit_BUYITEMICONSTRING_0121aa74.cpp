// dynamic initializer for CCharString `anonymous namespace'::BUY_ITEM_ICON_STRING @ 0x013B8B8C -- constructs it from the literal, then registers its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121AA74, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push -1 ; push 0x1253980 ; mov ecx, 0x13b8b8c ; call 0x99ebf0 ; push 0x1228f93 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CCharString
// living at 0x013B8B8C: it constructs it from the literal and registers the
// destructor thunk @ 0x01228F93 (`mov ecx,offset object ; jmp CCharString::~CCharString`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is its PDB name
// (see Names below). One of 46 bodies of this shape.
#include "fable_string.h"      // CCharString: ctor @ 0x0099EBF0 / default @ 0x0099AED0, ~CCharString @ 0x0099EAE0

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for '`anonymous namespace'::BUY_ITEM_ICON_STRING'' (0x0100E2CC) +
//   FableWin.pdb (0x03FF26DC); trade TU: both PDBs run SPEC_SUBTITLE .. CATEGORY_CHILD_SELL_LIST_STRING (14 packed
//   initializers) = retail 0x0121A935 .. 0x0121AAAE, literal for literal (Spec_Subtitle .. Category_Child_Sell_List). Anniv360
//   Test PDB (third source): S_LDATA32 CCharString BUY_ITEM_ICON_STRING (internal linkage). Was g_CCharString_013b8b8c.
namespace {
CCharString BUY_ITEM_ICON_STRING("Buy_Item_Icon", -1);    // 0x013B8B8C
}
