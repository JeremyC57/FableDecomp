// CTCBulletTime_Methods::GetBulletTimeRemainingPercentage() const -- the fraction of bullet time left: BulletTimeRemainingTicks / ceil(ticks-per-second * m_fDuration)
// Leaf: GetBulletTimeRemainingPercentage
// Retail-asm: 0x0079C6A6..0x0079C6B5 `fld [ebp-10h] ; fadd [ebp-0Ch] ; fistp dword ptr [ebp-4] ; fild [ebp-4] ; fsub [ebp-8] ; fstp [ebp-14h]` -- the engine's inline-assembly ceiling helper expanded in place (0.5f / 1.0f stored as frame constants, a bare dword fistp into a named long). VC7.1 emits no bare dword fistp from C++ (`(long)x` calls __ftol2, /QIfist gives `fistp qword`, /arch:SSE2 cvttss2si: rasm1 probe family (a)); the fix-up `mov eax,value ; cmp eax,back ; jne ; dec dword ptr` is part of the same block (10 instructions, one jne: rasm1 family (b)), the result read back by C++.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0079C680, 77 bytes (float result in st0: `fild remaining ; fidiv ticks`). Globals:
// 0x01375550 = the game's ticks-per-second long. BulletTimeRemainingTicks +0x0c, m_fDuration +0x2c (engine header). The
// remaining count is copied to a frame word before the asm barrier (the `fild` needs it in memory after the block).
#pragma optimize("y", off)
#include "engine/CTCBulletTime.h"
extern long g_TicksPerSecond;                       // 0x01375550
struct CTCBulletTime_Methods : CTCBulletTime {       // the PDB layout header carries no methods
    float GetBulletTimeRemainingPercentage() const;
};
// the engine's inline-assembly FPU ceiling helper (0x004FA9A0 is its standalone copy): VC7.1 expands the __forceinline body in
// place; the by-value parameter and the four locals become frame words, which is why the function carries an ebp frame.
// The whole 10-instruction block is asm (the bit compare and the memory decrement included: rasm1 family (b)); the result
// is read back by C++ after the block.
__forceinline long CeilToLong(float value)
{
    long  result;
    float one;
    float half;
    float back;
    half = 0.5f;
    one = 1.0f;
    __asm {
        fld   value
        fadd  half
        fistp result
        fild  result
        fsub  one
        fstp  back
        mov   eax, value
        cmp   eax, back
        jne   done
        dec   result
    done:
    }
    return result;
}
float CTCBulletTime_Methods::GetBulletTimeRemainingPercentage() const
{
    long remaining = BulletTimeRemainingTicks;
    return (float)remaining / CeilToLong(g_TicksPerSecond * m_fDuration);
}