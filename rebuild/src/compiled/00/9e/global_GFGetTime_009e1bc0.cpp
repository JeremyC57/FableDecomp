// GFGetTime() -- seconds since InitialiseTimerBase: the performance counter delta over its frequency, or (without a usable counter) the timeGetTime delta in milliseconds * 0.001
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E1BC0, length 168 bytes (+ int3 padding).
// PDB (FableWin): ?GFGetTime@@YINXZ -- __fastcall, no arguments, double result on the FPU.
// Callees: KERNEL32 QueryPerformanceCounter (IAT 0x0143FE00, loaded once into esi and called twice: dllimport),
// QueryPerformanceFrequency (IAT 0x0143FE04), WINMM timeGetTime (IAT 0x014404B4).
// Globals: the start count written by InitialiseTimerBase (0x009E1CB0, landed) at 0x013CB2F0/0x013CB2F4, and three
// statics of this function's module: "counter usable" byte 0x013CAA00, the counter frequency 0x013CAA08 and the
// "initialised" byte 0x013CAA10. Constant: double 0.001 at 0x0126F430.
// Shape: the one-time probe stores `QueryPerformanceCounter(&count) == 1` (`dec ; neg ; sbb al,al ; inc al`) before
// asking for the frequency; the counter path subtracts the 64-bit start and divides two `fild qword` values; the
// fallback zero-extends the 32-bit tick (`xor ecx,ecx ; sub ; sbb`) and multiplies by the 0.001 literal. The delta is
// a separate `__int64 elapsed` local: `count.QuadPart -= start` swaps the register pairs (edx:eax / esi:ecx).
union _LARGE_INTEGER { struct { unsigned long LowPart; long HighPart; } u; __int64 QuadPart; };
extern "C" __declspec(dllimport) int __stdcall QueryPerformanceCounter(_LARGE_INTEGER* count);       // IAT 0x0143FE00
extern "C" __declspec(dllimport) int __stdcall QueryPerformanceFrequency(_LARGE_INTEGER* frequency); // IAT 0x0143FE04
extern "C" __declspec(dllimport) unsigned long __stdcall timeGetTime();                               // IAT 0x014404B4

extern __int64 g_TimerStartCount_013cb2f0;        // 0x013CB2F0 / 0x013CB2F4 (InitialiseTimerBase)

static bool g_HasPerformanceCounter;              // 0x013CAA00
static _LARGE_INTEGER g_PerformanceFrequency;     // 0x013CAA08
static bool g_TimerInitialised;                   // 0x013CAA10

double __fastcall GFGetTime()
{
    if (!g_TimerInitialised)
    {
        _LARGE_INTEGER probe;
        g_HasPerformanceCounter = QueryPerformanceCounter(&probe) == 1;
        QueryPerformanceFrequency(&g_PerformanceFrequency);
        g_TimerInitialised = true;
    }
    if (g_HasPerformanceCounter)
    {
        _LARGE_INTEGER count;
        QueryPerformanceCounter(&count);
        __int64 elapsed = count.QuadPart - g_TimerStartCount_013cb2f0;
        return (double)elapsed / (double)g_PerformanceFrequency.QuadPart;
    }
    return (double)((__int64)timeGetTime() - g_TimerStartCount_013cb2f0) * 0.001;
}