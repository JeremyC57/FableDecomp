// CTCWobble::SetWobbleParameters(float seconds, float angle) -- store the wobble angle and the wobble duration in frames (seconds * frames-per-second, rounded up)
// Leaf: SetWobbleParameters
// Retail-asm: 0x007E2DC6..0x007E2DD5 `fld [ebp-8] ; fadd [ebp-4] ; fistp dword ptr [ebp+0Ch] ; fild [ebp+0Ch] ; fsub [ebp+8] ; fstp [ebp-0Ch]` -- the engine's inline-assembly ceiling helper expanded in place (0.5f in a frame word, 1.0f and the rounded long homed in the dead `seconds` / `angle` argument slots, a bare dword fistp). VC7.1 emits no bare dword fistp from C++ (`(long)x` calls __ftol2, /QIfist gives `fistp qword`, /arch:SSE2 cvttss2si: rasm1 probe family (a)); the fix-up `mov eax,value ; cmp ; jne ; dec dword ptr [ebp+0Ch]` is part of the same block (10 instructions, one jne: rasm1 family (b)), the result read back by C++.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E2DA0, 79 bytes (`ret 8`). Ego_r.pdb ?SetWobbleParameters@CTCWobble@@QAEXMM@Z.
// Layout (Ego_r.pdb CTCWobble, 48 bytes; the landed 0x007E2F00 model): FramesOfWobbleRemaining +0x24, DirectionLastRotated
// +0x28, AngleOfWobble +0x2c. Global 0x01375550 = frames per second (long).
#pragma optimize("y", off)
extern long g_FramesPerSecond;                              // 0x01375550
class CTCWobble {
public:
    unsigned char _pad00[0x24];
    long  FramesOfWobbleRemaining;                          // +0x24
    long  DirectionLastRotated;                             // +0x28
    float AngleOfWobble;                                    // +0x2c
    void SetWobbleParameters(float seconds, float angle);
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
void CTCWobble::SetWobbleParameters(float seconds, float angle)
{
    AngleOfWobble = angle;
    FramesOfWobbleRemaining = CeilToLong(g_FramesPerSecond * seconds);
}