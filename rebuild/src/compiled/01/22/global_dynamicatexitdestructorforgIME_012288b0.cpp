// `dynamic atexit destructor for 'gIME''() -- destroy the global NUISystem::CIME object gIME at exit (its inline destructor: Shutdown)
// Leaf: _$E2
// Retail: Fable.exe (TLC, VC7.1) @ 0x012288B0, 10 bytes: `mov ecx,0x013B7CD8 ; jmp 0x004051C0`. Registered with atexit by the dynamic
//   initializer 0x01219E90 (`push 0x012288B0 ; call atexit`), whose TU emits this function as its `_$E2`.
// Identity (proven): Ego_r.pdb 0x0101B2F0 / FableWin.pdb 0x0400E790 `dynamic atexit destructor for 'gIME'' (the initializer is Ego_r
//   0x0100C240 / FableWin 0x03FDDF10); 0x004051C0 is the landed ?Shutdown@CIME@NUISystem@@QAEXXZ (Ego_r / FableWin), which retail's
//   inline ~CIME expands to (FableWin.pdb keeps ??1CIME@NUISystem@@QAE@XZ out of line at 0x018BE950).
// agentNF193 (q4133, "likewise 0x012288B0 for gIME"): the landed stand-in (GlobalMethodTailThunk_012288b0 on a local AutoTinyThunkTarget,
//   g_AutoTinyThunkObject_013b7cd8 / Invoke_004051c0) is replaced by the definition of the global on the shared header, the same source
//   as the landed initializer 0x01219E90 (the compiler emits both functions from this one definition). The global is also defined by
//   that initializer row (the known initializer / atexit-destructor LNK2005, EXTRA_RULES 2026-11-20).
#define FABLE_RETAIL_CIME
#include "fable_ime.h"

NUISystem::CIME gIME;                                     // 0x013B7CD8
