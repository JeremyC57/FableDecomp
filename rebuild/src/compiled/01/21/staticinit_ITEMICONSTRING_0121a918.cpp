// dynamic initializer for CCharString `anonymous namespace'::ITEM_ICON_STRING @ 0x013B8B58 -- constructs it from the literal, then registers its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121A918, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push -1 ; push 0x1252150 ; mov ecx, 0x13b8b58 ; call 0x99ebf0 ; push 0x1228f0a ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CCharString
// living at 0x013B8B58: it constructs it from the literal and registers the
// destructor thunk @ 0x01228F0A (`mov ecx,offset object ; jmp CCharString::~CCharString`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object is the PDB's file-static
// `anonymous namespace'::ITEM_ICON_STRING (naming wave 15). One of 46 bodies of this shape.
#include "fable_string.h"      // CCharString: ctor @ 0x0099EBF0 / default @ 0x0099AED0, ~CCharString @ 0x0099EAE0

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for '`anonymous namespace'::ITEM_ICON_STRING'' (0x0100E170) + FableWin.pdb
//   (0x03FE4F51); CTCInventory TU: both PDBs run CTCInventory::ContextSensitivePreferredItems, TITLE_NAME,
//   ITEM_DESCRIPTION_STRING, ITEM_NAME_TABLE_STRING, ITEM_ICON_STRING = retail 0x0121A8B5 .. 0x0121A918; the source already
//   spelled it (naming wave 15), only the catalog name was the placeholder. Anniv360 Test PDB (third source): S_LDATA32
//   CCharString ITEM_ICON_STRING (internal linkage). Was g_CCharString_013b8b58.
namespace {
CCharString ITEM_ICON_STRING("Item_Icon", -1);    // 0x013B8B58
}
