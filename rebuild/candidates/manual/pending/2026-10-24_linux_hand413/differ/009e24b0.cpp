// GFRecalibrateFastSubFrameTimer() -- once at least a second has passed, re-measure the CPU cycles per second that the fast sub-frame timer divides by
// Grade: DIFFER(122v139) -- retail_asm candidate: retail reads the time-stamp counter inline (`xor eax,eax ; xor edx,edx ; rdtsc`, edx:eax spilled to the frame and reloaded); this C++ reference calls an out-of-line GFGetCycleCount instead. Everything else (compare, 64-bit subtract, fild/fdiv, the four global stores, the time store) is byte-identical around it.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E24B0, length 139 bytes. PDB: ?GFRecalibrateFastSubFrameTimer@@YIXXZ (void __fastcall()).
// Retail-asm evidence: 0x009E24D5 `xor eax,eax` / 0x009E24D7 `xor edx,edx` / 0x009E24D9 `rdtsc` then `mov [esp+8],eax ; mov [esp+0Ch],edx`
//   and a reload -- the same inline-asm block as the landed retail_asm CPUAheadCounterCallback (0x009E2540), i.e. the PDB's
//   GFGetCycleCount inlined; VC7.1 has no __rdtsc intrinsic, so the original ran inline asm here. Landing needs the owner's
//   retail_asm grade (C++ body + a minimal inline-asm block `xor eax,eax ; xor edx,edx ; rdtsc ; mov ...`), which this lane may not author.
// Callees: GFGetTime @ 0x009E1BC0 (double). Globals: last recalibration time 0x013CAA20 (double), last recalibration cycle
//   count 0x013CAA18 (64-bit), frame-start cycle count 0x013CA9F8 (64-bit), cycles per second 0x01396680 (double); 1.0 is the
//   double pool constant 0x01237F00.
// Shape: `elapsed < 1.0` early return (`test ah,5 ; jnp`), the signed 64-bit delta converted with `fild qword`, the counter
//   published to both cycle globals, the time stored last.
// Leaf: ?GFRecalibrateFastSubFrameTimer@@YIXXZ
typedef unsigned __int64 UHUGE;

double __fastcall GFGetTime();                            // 0x009E1BC0
UHUGE __fastcall GFGetCycleCount();                       // retail: inline `xor eax,eax ; xor edx,edx ; rdtsc` (inline asm)

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
    __int64 delta = (__int64)(cycles - g_LastRecalibrateCycleCount);
    g_FrameStartCycleCount = cycles;
    g_CyclesPerSecond = (double)delta / elapsed;
    g_LastRecalibrateCycleCount = cycles;
    g_LastRecalibrateTime = now;
}
