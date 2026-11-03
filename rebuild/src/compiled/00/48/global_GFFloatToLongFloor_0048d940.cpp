// GFFloatToLongFloor(float value) -- floor(value) as a long through the engine's inline-assembly FPU floor helper: round (value - 0.5) to nearest, then step up one when (result + 1) equals value exactly
// Leaf: GFFloatToLongFloor
// Retail-asm: 0x0048D954..0x0048D96E `fld [ebp+8] ; fsub [ebp-0Ch] ; fistp dword ptr [ebp-4] ; fild [ebp-4] ; fadd [ebp-8] ; fstp [ebp-10h]` on named memory locals (0.5f / 1.0f stored as frame constants, a bare dword fistp into a named long): VC7.1 emits no bare dword fistp from C++ (`(long)x` calls __ftol2, /QIfist gives `fistp qword`, /arch:SSE2 gives cvttss2si -- rasm1 probe family (a)), so the block is the original's inline-asm, including its fix-up (`mov eax,[ebp+8] ; cmp eax,[ebp-10h] ; jne ; inc dword ptr [ebp-4]`: a memory increment VC7.1 would hoist into a register from C++) -- rasm1 family (b), 10 instructions with one jne.
// Helper (2026-11-03 asm twins): the retail inline-asm block is the shared engine/GFFloatToLongFloor.h (two-path: the byte-matching asm, and a FABLE_PORTABLE C++ twin); this row carries no asm of its own.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0048D940, 58 bytes, `?GFFloatToLongFloor@@YIJM@Z` (__fastcall, the float on the
// stack, `ret 4`); no callers in the image (the callers inline the helper, e.g. 0x00A52650). The standalone twin of the
// ceiling helper landed at 0x004FA9A0. The inline asm on frame locals forces the ebp frame.
// The out-of-line copy of the header's __forceinline helper: retail's 0x0048D940 has no callers (every caller expands the
// block in place), so this TU takes the helper's address to emit the COMDAT, as the landed 0x005BC3ED row does.
#pragma optimize("y", off)
#include "engine/GFFloatToLongFloor.h"
#ifndef FABLE_PORTABLE   // the retail out-of-line COMDAT; a portable build needs no copy
long (__fastcall *g_keepGFFloatToLongFloor_0048D940)(float) = &GFFloatToLongFloor;
#endif
