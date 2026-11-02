// `dynamic initializer for 'gIME''() -- construct the global NUISystem::CIME object gIME (its inline ctor resets the default IME appearance) and register its destructor (inline: Shutdown) with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01219E90, 17 bytes, .CRT$XCU entry 0x01374208 (the first of the table):
//   `call 0x00404A90 ; push 0x012288B0 ; call atexit ; pop ecx ; ret`; the atexit thunk 0x012288B0 is this TU's `_$E2`
//   (`mov ecx,0x013B7CD8 ; jmp 0x004051C0`), reproduced by the same source.
// Identity (proven): Ego_r.pdb 0x0100C240 / FableWin.pdb 0x03FDDF10 `dynamic initializer for 'gIME'' (+ `dynamic atexit destructor
//   for 'gIME'' 0x0101B2F0 / 0x0400E790), the first game initializer after `_iob' in Ego_r as in retail's table; the object at
//   0x013B7CD8 is what CIME::Get 0x00404A80 returns (landed, Ego_r/FableWin ?Get@CIME@NUISystem@@SIAAV12@XZ); 0x00404A90 is the landed
//   ?ResetToDefaultSettings@CIME@NUISystem@@SIXXZ (static __fastcall: no `this`), 0x004051C0 the landed ?Shutdown@CIME@NUISystem@@QAEXXZ.
//   FableWin.pdb has the ctor/dtor out of line (??0CIME@NUISystem@@QAE@XZ 0x018BE930, ??1CIME@NUISystem@@QAE@XZ 0x018BE950); Ego_r and
//   retail expand them: ctor = ResetToDefaultSettings() with no member store, dtor = Shutdown().
// Header: fable_ime.h retail opt-in FABLE_RETAIL_CIME (hdr/ overlay, chunk 716): PDB member names, the inline ctor / dtor, the statics.
// Linkage: gIME is written as an external global (`?gIME@@3VCIME@NUISystem@@A`); a file `static` would compile to the same bytes and
//   is not excluded (every reader is in the CIME TU) -- see NOTES.md.
#define FABLE_RETAIL_CIME
#include "fable_ime.h"

NUISystem::CIME gIME;                                     // 0x013B7CD8