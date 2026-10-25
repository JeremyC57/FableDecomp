// GFRecalibrateFastSubFrameTimer() -- once at least a second has passed, re-measure the CPU cycles per second that the fast sub-frame timer divides by
// Grade: RELOCATION_MATCH (2 grades: the delta written inline in the division) -- rasm1 retail_asm: C++ body + a minimal 5-instruction __asm (no control flow) + the Retail-asm evidence line
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E24B0, length 139 bytes. PDB: ?GFRecalibrateFastSubFrameTimer@@YIXXZ (void __fastcall()).
// Retail-asm: 0x009E24D5 `xor eax,eax` / 0x009E24D7 `xor edx,edx` / 0x009E24D9 `rdtsc` with edx:eax stored to the 8-byte local (0x009E24DB/0x009E24DF) and reloaded -- the PDB's GFGetCycleCount inlined, the same block as the landed CPUAheadCounterCallback (0x009E2540); VC7.1 (13.10.3077/4035) has no rdtsc intrinsic (__rdtsc/<intrin.h> arrived with VC8), so the original read the counter with inline asm.
// Callees: GFGetTime @ 0x009E1BC0 (double). Globals: last recalibration time 0x013CAA20 (double), last recalibration cycle
//   count 0x013CAA18 (64-bit), frame-start cycle count 0x013CA9F8 (64-bit), cycles per second 0x01396680 (double); 1.0 is the
//   double pool constant 0x01237F00.
// Shape: `elapsed < 1.0` early return (`test ah,5 ; jnp`), the signed 64-bit delta converted with `fild qword`, the counter
//   published to both cycle globals, the time stored last.
// Leaf: ?GFRecalibrateFastSubFrameTimer@@YIXXZ
typedef unsigned __int64 UHUGE;

double __fastcall GFGetTime();                            // 0x009E1BC0
// the engine's cycle-counter read (expanded in place: zero edx:eax, rdtsc, spill to the 8-byte local)
__forceinline UHUGE GFGetCycleCount()
{
    UHUGE counter;
    __asm {
        xor eax, eax
        xor edx, edx
        rdtsc
        mov dword ptr counter, eax
        mov dword ptr counter+4, edx
    }
    return counter;
}

extern double g_LastRecalibrateTime;                      // 0x013CAA20
extern UHUGE g_LastRecalibrateCycleCount;                 // 0x013CAA18
extern UHUGE g_FrameStartCycleCount;                      // 0x013CA9F8
extern double g_CyclesPerSecond;                          // 0x01396680

void __fastcall GFRecalibrateFastSubFrameTimer()
{
    double now = GFGetTime();
    double elapsed = now - g_LastRecalibrateTime;
    if (elapsed < 1.0)
        return;
    UHUGE cycles = GFGetCycleCount();
    g_CyclesPerSecond = (double)(__int64)(cycles - g_LastRecalibrateCycleCount) / elapsed;
    g_FrameStartCycleCount = cycles;
    g_LastRecalibrateCycleCount = cycles;
    g_LastRecalibrateTime = now;
}