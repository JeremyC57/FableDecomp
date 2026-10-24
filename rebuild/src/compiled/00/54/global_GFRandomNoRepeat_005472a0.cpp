// GFRandomNoRepeat(unsigned long range, unsigned long previous, unsigned long* seed) -- draw a random index below range from the seed, re-drawing while it equals the previous pick (only when range > 1), giving up with a debug message after 100 extra draws
// Leaf: ?GFRandomNoRepeat@
// Retail: Fable.exe (TLC, VC7.1) @ 0x005472A0, 134 bytes, `ret 4` (Ego_r.pdb `?GFRandomNoRepeat@@YIKKKPAK@Z`: a free
//   __fastcall, range in ecx, previous in edx, seed on the stack).
// Callees: GFROR13 0x00497890 (__fastcall rotate right by 13), OutputDebugStringW (KERNEL32 import, IAT 0x0143FE14)
//   with L"GFRandomNoRepeat is going on a bit - breaking the loop" (0x01248508).
// Callers: one call at 0x00547466 (NUISystem::CSwappingStateComponent slot 1, 0x00547380).
// Shape: GFRandom (`*seed = *seed * 0x24a1 + 0x24df; *seed = GFROR13(*seed); return range ? *seed % range : 0;`, as in
//   the landed CTCUserActionPointBase::GetFreeActionPoint 0x007FEDE0) is `__forceinline`; the redraw loop is
//   `while (range > 1 && result == previous)` with the attempt count initialised before it: the loop-invariant
//   `range > 1` is tested once at the entry and the result lives in ebx, previous in the frame slot. The `if (range > 1)
//   { while (result == previous) ... }` form keeps the first result in eax and previous in ebx (145v134).
extern "C" __declspec(dllimport) void __stdcall OutputDebugStringW(const unsigned short* text);   // KERNEL32 import (IAT 0x0143FE14); WCHAR is unsigned short (no /Zc:wchar_t)

unsigned long __fastcall GFROR13(unsigned long value);       // 0x00497890 (rotate right by 13)

__forceinline unsigned long GFRandom(unsigned long range, unsigned long* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return range ? *seed % range : 0;
}
unsigned long __fastcall GFRandomNoRepeat(unsigned long range, unsigned long previous, unsigned long* seed)
{
    unsigned long result = GFRandom(range, seed);
    long attempts = 0;
    while (range > 1 && result == previous) {
        result = GFRandom(range, seed);
        if (++attempts > 100) {
            OutputDebugStringW(L"GFRandomNoRepeat is going on a bit - breaking the loop");
            break;
        }
    }
    return result;
}