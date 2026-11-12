// dynamic initializer for CWideString `anonymous namespace'::MISSING_EMPTY_TEXT @ 0x013BAD9C -- constructs it from the literal, then registers its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121EEB0, oracle length 27 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push 0x126a44c ; mov ecx, 0x13bad9c ; call 0x99b6b0 ; push 0x1229ae0 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CWideString
// living at 0x013BAD9C: it default-constructs it out of line and registers the
// destructor thunk @ 0x01229AE0 (`mov ecx,offset object ; jmp CWideString::~CWideString`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is its PDB name
// (see Names below). One of 12 bodies of this shape.
#include "fable_string.h"      // CWideString: CWideString(const wchar_t*) @ 0x0099B6B0 (landed), ~CWideString

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for '`anonymous namespace'::MISSING_EMPTY_TEXT'' (0x01011870) +
//   FableWin.pdb (0x03FF4020); Ego_r runs MISSING_AUGMENTATION_TEXT, MISSING_EMPTY_TEXT = retail 0x0121EE90 (L"Missing
//   augmentation name!"), 0x0121EEB0 (L"Missing empty text!"). Anniv360 Test PDB (third source): S_LDATA32 CWideString
//   MISSING_EMPTY_TEXT (internal linkage). Was g_CWideString_013bad9c.
namespace {
CWideString MISSING_EMPTY_TEXT(L"Missing empty text!");    // 0x013BAD9C
}
