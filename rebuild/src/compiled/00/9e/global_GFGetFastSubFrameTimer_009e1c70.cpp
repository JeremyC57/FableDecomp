// GFGetFastSubFrameTimer() -- cycle-counter time: (rdtsc - base) / cycles-per-second, a cheap sub-frame timer
// Grade: RELOCATION_MATCH (1 grade) -- rasm1 retail_asm: C++ body + a minimal 5-instruction __asm (no control flow) + the Retail-asm evidence line
// Leaf: ?GFGetFastSubFrameTimer@@YINXZ
// Retail-asm: 0x009E1C73 `xor eax,eax` / 0x009E1C75 `xor edx,edx` / 0x009E1C77 `rdtsc` with edx:eax stored to the 8-byte local at [esp] (0x009E1C79/0x009E1C7C); VC7.1 (13.10.3077/4035) has no rdtsc intrinsic (__rdtsc/<intrin.h> arrived with VC8), so the original read the counter with inline asm (same block as the landed 0x009E2540 CPUAheadCounterCallback).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E1C70, length 58 bytes (+ int3 padding).
// PDB (FableWin): ?GFGetFastSubFrameTimer@@YINXZ -- __fastcall, no arguments, double result on the FPU.
// Globals: the 64-bit counter base at 0x013CA9F8 / 0x013CA9FC (written by the timer initialisation) and the double
// cycles-per-second at 0x01396680 (a .data global initialised to 1.0e9, not a literal-pool constant).
// Body after the asm block is ordinary C++: the 64-bit subtraction through the frame words, `fild qword [esp] ;
// fdiv qword [g]`.
extern __int64 g_FastTimerBase;           // 0x013CA9F8
extern double g_CyclesPerSecond;          // 0x01396680 (1.0e9)

double __fastcall GFGetFastSubFrameTimer()
{
    __int64 now;
    __asm {
        xor eax, eax
        xor edx, edx
        rdtsc
        mov dword ptr now, eax
        mov dword ptr now+4, edx
    }
    now -= g_FastTimerBase;
    return (double)now / g_CyclesPerSecond;
}