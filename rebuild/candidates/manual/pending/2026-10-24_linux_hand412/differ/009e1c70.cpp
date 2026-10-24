// GFGetFastSubFrameTimer() -- cycle-counter time: (rdtsc - base) / cycles-per-second, a cheap sub-frame timer
// Grade: DIFFER(40v58) -- retail_asm candidate: retail reads the time-stamp counter with inline asm (`xor eax,eax ; xor edx,edx ; rdtsc ; mov [esp],eax ; mov [esp+4],edx`), which VC7.1 cannot emit from C++ (no __rdtsc intrinsic before VC8); this C++ reference body stands the read in with an out-of-line call
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E1C70, length 58 bytes (+ int3 padding).
// PDB (FableWin): ?GFGetFastSubFrameTimer@@YINXZ -- __fastcall, no arguments, double result on the FPU.
// Globals: the 64-bit counter base at 0x013CA9F8 / 0x013CA9FC (written by the timer initialisation) and the double
// cycles-per-second at 0x01396680 (a .data global initialised to 1.0e9, not a literal-pool constant).
// Retail-asm: the body after the asm block is ordinary C++: `now -= base; return (double)now / g_CyclesPerSecond;`
// (the 64-bit subtraction through the frame words, `fild qword [esp] ; fdiv qword [g]`). Landing needs the
// `retail_asm` grade (a C++ body with the minimal inline-asm block `{ xor eax,eax / xor edx,edx / rdtsc / mov now,eax /
// mov now+4,edx }` and a `// Retail-asm:` line), which this lane's agents may not author.
extern __int64 g_FastTimerBase;           // 0x013CA9F8
extern double g_CyclesPerSecond;          // 0x01396680 (1.0e9)
extern "C" __int64 __cdecl ReadTimeStampCounter();   // stands for the retail inline `rdtsc`

double __fastcall GFGetFastSubFrameTimer()
{
    __int64 now = ReadTimeStampCounter();
    now -= g_FastTimerBase;
    return (double)now / g_CyclesPerSecond;
}
