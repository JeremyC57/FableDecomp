// _global::RandomBelow_00c42a70(unsigned long n) -- a pseudo-random index in [0, n): rand() scaled by n / 32768, truncated
// Leaf: ?RandomBelow_00c42a70@@YIKK@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C42A20, 53 bytes, __fastcall (n in ecx), tail jump to __ftol2.
// Identity: libpk helper of the landed CShuffleBag_00c42a70::PickNext (its binding spelling kept).
// Callees: MSVCR71 rand (IAT 0x01440124, dllimport), __ftol2.
// Shape: `(unsigned long)(rand() * (double)n * (1.0 / 32768))`.
extern "C" __declspec(dllimport) int __cdecl rand(void);   // MSVCR71 import (IAT 0x01440124)
unsigned long __fastcall RandomBelow_00c42a70(unsigned long n)
{
    return (unsigned long)(rand() * (double)n * (1.0 / 32768));
}