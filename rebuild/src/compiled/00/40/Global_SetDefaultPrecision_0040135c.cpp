#include <float.h>

// __setdefaultprecision() -- CRT: put the x87 FPU into 53-bit (double) precision at start-up
// Library: msvcrt.lib(build\intel\dll_obj\fp8.obj) __setdefaultprecision
// Retail: Fable.exe (TLC, VC7.1) @ 0x0040135C, 18 bytes. Callees: _controlfp (import).
//
// Windows starts a process with the FPU in 64-bit (extended) precision. The C runtime's start-up code
// calls this to select 53-bit precision (`_PC_53` = 0x10000 under the `_MCW_PC` = 0x30000 mask) so that
// `double` arithmetic rounds the same way the compiler assumed. Nothing game-specific lives here, but the
// whole engine's float behaviour, including the deterministic parts of physics and animation, depends on
// this having run first.
//
// Matching: `optimize("s")` so the two pushed arguments are discarded with `pop ecx; pop ecx` (2 bytes)
// rather than `add esp,8` (3 bytes), which is the idiom in this start-up object.
#pragma optimize("s", on)

extern "C" void __cdecl __setdefaultprecision()
{
    _controlfp(0x10000, 0x30000);
}
