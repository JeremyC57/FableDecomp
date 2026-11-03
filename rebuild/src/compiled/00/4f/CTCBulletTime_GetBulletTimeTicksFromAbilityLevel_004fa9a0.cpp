// GFFloatToLongCeil(float value) -- ceil(value) as a long: the out-of-line copy of the engine's inline-assembly ceiling helper (engine/GFFloatToLongCeil.h)
// Leaf: GFFloatToLongCeil
// Retail-asm: 0x004FA9B4..0x004FA9CE `fld [ebp+8] ; fadd [ebp-0Ch] ; fistp dword ptr [ebp-4] ; fild [ebp-4] ; fsub [ebp-8] ; fstp [ebp-10h] ; mov eax,[ebp+8] ; cmp eax,[ebp-10h] ; jne ; dec dword ptr [ebp-4]` on named memory locals (0.5f / 1.0f stored as frame constants, a bare dword fistp into a named long, the fix-up on memory): VC7.1 emits no bare dword fistp from C++ (/QIfist gives `fistp qword`), so the block is the original's inline asm.
// Helper (2026-11-03 asm twins): the retail inline-asm block is the shared engine/GFFloatToLongCeil.h (two-path: the byte-matching asm, and a FABLE_PORTABLE C++ twin); this row carries no asm of its own.
// Retail 0x004FA9A0, 58 bytes (`ret 4`, ecx unused, no direct callers). Identity (2026-11-03 asm twins, NAME_FIXES): the
// catalog's CTCBulletTime::GetBulletTimeTicksFromAbilityLevel is wrong -- the PDB spells that member
// ?GetBulletTimeTicksFromAbilityLevel@CTCBulletTime@@ABEJXZ (no argument, plain `ret`: landed 0x0079C5F0, which inlines
// this block). This body takes one float on the stack and is the debug-frame copy of FableWin.pdb
// ?GFFloatToLongCeil@@YIJM@Z (58 bytes, the same size), instruction for instruction the floor copy GFFloatToLongFloor
// 0x0048D940 with fadd/fsub swapped and `inc` -> `dec`.
// The ceiling is the classic FPU trick: round (x + 0.5) to nearest with the default rounding mode, then step back one
// when (result - 1) equals x bit for bit (x was already integral and the half rounded up). The asm on frame locals
// forces the ebp frame (`push ebp ; mov ebp,esp ; sub esp,10h`); the TU takes the helper's address to emit the COMDAT.
#pragma optimize("y", off)
#include "engine/GFFloatToLongCeil.h"
#ifndef FABLE_PORTABLE   // the retail out-of-line COMDAT; a portable build needs no copy
long (__fastcall *g_keepGFFloatToLongCeil_004FA9A0)(float) = &GFFloatToLongCeil;
#endif
