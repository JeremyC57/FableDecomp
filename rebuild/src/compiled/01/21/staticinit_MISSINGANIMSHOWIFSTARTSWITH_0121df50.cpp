// dynamic initializer for CCharString `anonymous namespace'::MISSING_ANIM_SHOW_IF_STARTS_WITH @ 0x013BA87C -- constructs it from the literal, then registers its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121DF50, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push -1 ; push 0x125b16c ; mov ecx, 0x13ba87c ; call 0x99ebf0 ; push 0x12291e0 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CCharString
// living at 0x013BA87C: it constructs it from the literal and registers the
// destructor thunk @ 0x012291E0 (`mov ecx,offset object ; jmp CCharString::~CCharString`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is its PDB name
// (see Names below). One of 163 bodies of this shape.
#include "fable_string.h"      // CCharString: ctor @ 0x0099EBF0 / default @ 0x0099AED0, ~CCharString @ 0x0099EAE0

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for '`anonymous namespace'::MISSING_ANIM_SHOW_IF_STARTS_WITH''
//   (0x0100CBC0) + FableWin.pdb (0x03FDFC60); Ego_r runs REACTION_MANAGER_DEF_INSTANTIATION_NAME,
//   MISSING_ANIM_SHOW_IF_STARTS_WITH, MISSING_ANIM_EMOTE_TEXT = retail 0x0121DF30, 0x0121DF50 ("ST_OPINION_"), 0x0121DF70
//   (L"MISSING OPINION ANIM"). Anniv360 Test PDB (third source): S_LDATA32 CCharString MISSING_ANIM_SHOW_IF_STARTS_WITH
//   (internal linkage). Was g_CCharString_013ba87c.
namespace {
CCharString MISSING_ANIM_SHOW_IF_STARTS_WITH("ST_OPINION_", -1);    // 0x013BA87C
}
