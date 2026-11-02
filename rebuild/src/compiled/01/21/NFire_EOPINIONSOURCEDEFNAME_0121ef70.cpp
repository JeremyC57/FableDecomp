// NFire::$E_OPINION_SOURCE_DEF_NAME() -- dynamic initializer of the const CCharString& NFire::OPINION_SOURCE_DEF_NAME bound to a temporary built from "OPINION_SOURCE_SCARY_FIRE": register the temporary's atexit destructor, construct it, store its address in the reference
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121EF70, 41 bytes, in `.text$yc`, reached from the .CRT$XCU table entry
//   0x013745D0, plain `ret`: `push 0x01229B40 ; call _atexit ; add esp,4 ; push -1 ; push "OPINION_SOURCE_SCARY_FIRE" ;
//   mov ecx,0x013BADD8 ; call 0x0099EBF0 ; mov [0x013BADE0],0x013BADD8`.
// Identity (proven, two sources): (1) FableWin.pdb 0x03FE17A0 / 0x03FE17D0 runs, in one TU,
//   REACTION_MANAGER_DEF_INSTANTIATION_NAME -> NFire::`dynamic initializer for 'OPINION_SOURCE_DEF_NAME'' (a namespace-scope
//   object of NFire, the fire namespace of NFire::CFirePoint and CTCFlammable); retail runs 0x0121EF50 ("OPINION_REACTION_
//   MANAGER_INSTANCE" at 0x013BADDC) -> this; (2) the object's one reader is CTCFlammable at 0x00780D80 (0x00780EA6), which
//   passes `[0x013BADE0]` as the `const CCharString&` name argument of COpinionReactionManager::AttachOpinionSource 0x00700780
//   (?AttachOpinionSource@COpinionReactionManager@@QAE_NPAVCThing@@ABVCCharString@@PAPAVCTCOpinionSource@@@Z): the fire's
//   "scary fire" opinion source def. Ego_r.pdb predates the object (no entry). Catalog row "_gapscan::sub_0121ef70" carried no
//   name; the atexit thunk 0x01229B40 (`mov ecx,0x013BADD8 ; jmp ~CCharString`) is landed as the stand-in
//   `GlobalMethodTailThunk01229b40` (label `AtexitDtor_CCharString_at_013badd8`).
// Shape: a namespace-scope reference bound to a temporary: VC7.1 registers the lifetime-extended temporary's destructor
//   (`_$E3`, destroying the internal `_$S2` at 0x013BADD8) BEFORE constructing it, then stores the temporary's address in the
//   reference (0x013BADE0) -- exactly retail's order; a plain `const CCharString` object constructs first and has no pointer.
// Data symbol: ?OPINION_SOURCE_DEF_NAME@NFire@@3ABVCCharString@@B (the reference, 0x013BADE0); the temporary is the compiler's
//   TU-local `_$S2` (0x013BADD8), never a bindable name.
// Callees: _atexit 0x004012BC, CCharString::CCharString(const char*, long = -1) 0x0099EBF0 (??0CCharString@@QAE@PBDJ@Z);
//   ~CCharString 0x0099EAE0 in the `_$E3` thunk (not this row). Header: fable_string.h with the FABLE_RETAIL_CCHARSTRING opt-in
//   (the literal converts implicitly through the defaulted length, hdr/README.md).
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"      // CCharString(const char*, long = -1) @ 0x0099EBF0, ~CCharString @ 0x0099EAE0

namespace NFire {
const CCharString& OPINION_SOURCE_DEF_NAME = "OPINION_SOURCE_SCARY_FIRE";   // 0x013BADE0 (temporary 0x013BADD8)
}