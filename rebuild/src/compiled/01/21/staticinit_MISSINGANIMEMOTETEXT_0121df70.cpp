// dynamic initializer for CWideString `anonymous namespace'::MISSING_ANIM_EMOTE_TEXT @ 0x013BA880 -- constructs it from the literal, then registers its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121DF70, oracle length 27 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push 0x125b178 ; mov ecx, 0x13ba880 ; call 0x99b6b0 ; push 0x12291f0 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CWideString
// living at 0x013BA880: it default-constructs it out of line and registers the
// destructor thunk @ 0x012291F0 (`mov ecx,offset object ; jmp CWideString::~CWideString`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is its PDB name
// (see Names below). One of 12 bodies of this shape.
#include "fable_string.h"      // CWideString: CWideString(const wchar_t*) @ 0x0099B6B0 (landed), ~CWideString

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for '`anonymous namespace'::MISSING_ANIM_EMOTE_TEXT'' (0x0100CBE0) +
//   FableWin.pdb (0x03FDFC90); Ego_r runs REACTION_MANAGER_DEF_INSTANTIATION_NAME, MISSING_ANIM_SHOW_IF_STARTS_WITH,
//   MISSING_ANIM_EMOTE_TEXT = retail 0x0121DF30, 0x0121DF50 ("ST_OPINION_"), 0x0121DF70 (L"MISSING OPINION ANIM"). Anniv360
//   Test PDB (third source): S_LDATA32 CWideString MISSING_ANIM_EMOTE_TEXT (internal linkage). Was g_CWideString_013ba880.
namespace {
CWideString MISSING_ANIM_EMOTE_TEXT(L"MISSING OPINION ANIM");    // 0x013BA880
}
