// InitialiseTimerBase() -- record the start count of the high-resolution timer (two QueryPerformanceCounter calls: the first probes availability) or, without one, the millisecond tick with a zero high word
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E1CB0, length 79 bytes.
// Callees: KERNEL32 QueryPerformanceCounter (IAT 0x0143FE00, used twice: `mov esi,[__imp] ; call esi`, so `__declspec(dllimport)`), WINMM timeGetTime
// (IAT 0x014404B4). Globals: the 64-bit start count at 0x013CB2F0 (low) / 0x013CB2F4 (high). Shape: an 8-byte LARGE_INTEGER local, `cmp eax,1` on the
// first BOOL, the 64-bit store as two dword moves, the fallback zero-extends the tick. The manifest label "CEditControlCentre::FlipSelectedTrack" is
// propagated; a free function with no arguments.
union _LARGE_INTEGER { struct { unsigned long LowPart; long HighPart; } u; __int64 QuadPart; };
extern "C" __declspec(dllimport) int __stdcall QueryPerformanceCounter(_LARGE_INTEGER* count);   // IAT 0x0143FE00
extern "C" __declspec(dllimport) unsigned long __stdcall timeGetTime();                           // IAT 0x014404B4

extern __int64 g_TimerStartCount_013cb2f0;        // 0x013CB2F0 / 0x013CB2F4

void InitialiseTimerBase()
{
    _LARGE_INTEGER count;
    if (QueryPerformanceCounter(&count) == 1) {
        QueryPerformanceCounter(&count);
        g_TimerStartCount_013cb2f0 = count.QuadPart;
    } else {
        g_TimerStartCount_013cb2f0 = timeGetTime();
    }
}