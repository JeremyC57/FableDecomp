// dynamic initializer for CCharString `anonymous namespace'::LINE_STRING_NAME @ 0x013BAD90 -- constructs it from the literal, then registers its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121EE50, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push -1 ; push 0x126a36c ; mov ecx, 0x13bad90 ; call 0x99ebf0 ; push 0x1229a90 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CCharString
// living at 0x013BAD90: it constructs it from the literal and registers the
// destructor thunk @ 0x01229A90 (`mov ecx,offset object ; jmp CCharString::~CCharString`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object is the PDB's file-static
// `anonymous namespace'::LINE_STRING_NAME (naming wave 15). One of 163 bodies of this shape.
#include "fable_string.h"      // CCharString: ctor @ 0x0099EBF0 / default @ 0x0099AED0, ~CCharString @ 0x0099EAE0

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for '`anonymous namespace'::LINE_STRING_NAME'' (0x01011810) + FableWin.pdb
//   (0x03FF3F90); Ego_r runs CLOSED_STRING_NAME, LINE_STRING_NAME, LEGACY_SHAPE_TYPE = retail 0x0121EE30, 0x0121EE50,
//   0x0121EE70; the source already spelled it (naming wave 15), only the catalog name was the placeholder. Anniv360 Test PDB
//   (third source): S_LDATA32 CCharString LINE_STRING_NAME (internal linkage). Was g_CCharString_013bad90.
namespace {
CCharString LINE_STRING_NAME("SHAPE_TYPE_LINE", -1);    // 0x013BAD90
}
