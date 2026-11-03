// FtoL(float value) -- round a float to a long with the FPU's current rounding mode (the engine's out-of-line float-to-long helper)
// Grade: MATCH (1 grade) -- rasm1 retail_asm: C++ body + a minimal 3-instruction inline-asm (no control flow) + the Retail-asm evidence line
// Leaf: FtoL
// Retail-asm: 0x0081E55A `fld dword ptr [esp+0Ch]` + 0x0081E55E `mov edx,[esp+4]` + 0x0081E562 `fistp dword ptr [edx]` -- the engine's inline-assembly FloatToLong(long* result, float value) helper (the same three instructions as the landed retail_asm 0x00B1B5E0), the result pointer homed at [esp+4] and reloaded into edx; VC7.1 C++ `(long)x` calls __ftol2 and /QIfist (RTM 3077 and QFE 4035, with /Op /Og /G5..G7) emits `fistp qword`, /arch:SSE2 a cvttss2si, so no C/C++ form gives a dword fistp.
// Helper (2026-11-03 asm twins): the retail inline-asm block is the shared engine/GFFastFloatToLong.h (two-path: the byte-matching asm, and a FABLE_PORTABLE C++ twin); this row carries no asm of its own.
// Retail: Fable.exe (TLC, VC7.1) @ 0x0081E550, 29 bytes, `ret 4` (one float on the stack; ecx unused). No callees, no code xrefs.
// the engine's pointer-out float-to-long helper: expanded in place, the pointer parameter gets a frame copy
#include "engine/GFFastFloatToLong.h"
long __stdcall FtoL(float value)
{
    long result;
    result = GFFastFloatToLong(value);
    return result;
}