// GFGetFastSubFrameTimer() -- cycle-counter time: (rdtsc - base) / cycles-per-second, a cheap sub-frame timer
// Grade: RELOCATION_MATCH -- retail_asm through the shared helper engine/GFGetCycleCount.h (2026-11-05 asm migration, docs/pipeline/ASM_RULES.md); no inline asm in this row
// Leaf: ?GFGetFastSubFrameTimer@@YINXZ
// Retail-asm: 0x009E1C73 `xor eax,eax` / 0x009E1C75 `xor edx,edx` / 0x009E1C77 `rdtsc` with edx:eax stored to the 8-byte local at [esp] (0x009E1C79/0x009E1C7C); VC7.1 (13.10.3077/4035) has no rdtsc intrinsic (__rdtsc/<intrin.h> arrived with VC8), so the original read the counter with inline asm (same block as the landed 0x009E2540 CPUAheadCounterCallback).
// Helper (2026-11-05 asm migration): the xor/xor/rdtsc block is engine/GFGetCycleCount.h (the GF_GET_CYCLE_COUNT macro); the row
//   has no asm of its own. C++/intrinsic attempts: the batch ATTEMPTS.tsv, family RDTSC.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E1C70, length 58 bytes (+ int3 padding).
// PDB (FableWin): ?GFGetFastSubFrameTimer@@YINXZ -- __fastcall, no arguments, double result on the FPU.
// Globals: the 64-bit counter base at 0x013CA9F8 / 0x013CA9FC (written by the timer initialisation) and the double
// cycles-per-second at 0x01396680 (a .data global initialised to 1.0e9, not a literal-pool constant).
// Body after the asm block is ordinary C++: the 64-bit subtraction through the frame words, `fild qword [esp] ;
// fdiv qword [g]`.
#include "engine/GFGetCycleCount.h"
extern __int64 g_FastTimerBase;           // 0x013CA9F8
extern double g_CyclesPerSecond;          // 0x01396680 (1.0e9)

double __fastcall GFGetFastSubFrameTimer()
{
    __int64 now;
    GF_GET_CYCLE_COUNT(now);
    now -= g_FastTimerBase;
    return (double)now / g_CyclesPerSecond;
}