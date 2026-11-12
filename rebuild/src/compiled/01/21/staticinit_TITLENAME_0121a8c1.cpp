// dynamic initializer for CCharString `anonymous namespace'::TITLE_NAME @ 0x013B8B4C -- constructs it from the literal, then registers its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121A8C1, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push -1 ; push 0x1252120 ; mov ecx, 0x13b8b4c ; call 0x99ebf0 ; push 0x1228eec ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CCharString
// living at 0x013B8B4C: it constructs it from the literal and registers the
// destructor thunk @ 0x01228EEC (`mov ecx,offset object ; jmp CCharString::~CCharString`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object is the PDB's file-static
// `anonymous namespace'::TITLE_NAME (naming wave 15). One of 46 bodies of this shape.
#include "fable_string.h"      // CCharString: ctor @ 0x0099EBF0 / default @ 0x0099AED0, ~CCharString @ 0x0099EAE0

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for '`anonymous namespace'::TITLE_NAME'' (0x0100DE7E/0x0100E119) +
//   FableWin.pdb (0x03FE0FF8/0x03FE4ED6); CTCInventory TU: both PDBs run CTCInventory::ContextSensitivePreferredItems,
//   TITLE_NAME, ITEM_DESCRIPTION_STRING, ITEM_NAME_TABLE_STRING, ITEM_ICON_STRING = retail 0x0121A8B5 .. 0x0121A918; the source
//   already spelled it (naming wave 15), only the catalog name was the placeholder. Anniv360 Test PDB (third source): S_LDATA32
//   CCharString TITLE_NAME (internal linkage). Was g_CCharString_013b8b4c.
namespace {
CCharString TITLE_NAME("ITEMS TITLE", -1);    // 0x013B8B4C
}
