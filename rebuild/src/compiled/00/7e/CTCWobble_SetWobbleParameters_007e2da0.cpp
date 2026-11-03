// CTCWobble::SetWobbleParameters(float seconds, float angle) -- store the wobble angle and the wobble duration in frames (seconds * frames-per-second, rounded up)
// Leaf: SetWobbleParameters
// Naming wave 18: the global stand-in g_FramesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail-asm: 0x007E2DC6..0x007E2DD5 `fld [ebp-8] ; fadd [ebp-4] ; fistp dword ptr [ebp+0Ch] ; fild [ebp+0Ch] ; fsub [ebp+8] ; fstp [ebp-0Ch]` -- the engine's inline-assembly ceiling helper expanded in place (0.5f in a frame word, 1.0f and the rounded long homed in the dead `seconds` / `angle` argument slots, a bare dword fistp). VC7.1 emits no bare dword fistp from C++ (`(long)x` calls __ftol2, /QIfist gives `fistp qword`, /arch:SSE2 cvttss2si: rasm1 probe family (a)); the fix-up `mov eax,value ; cmp ; jne ; dec dword ptr [ebp+0Ch]` is part of the same block (10 instructions, one jne: rasm1 family (b)), the result read back by C++.
// Helper (2026-11-03 asm twins): the retail inline-asm block is the shared engine/GFFloatToLongCeil.h (two-path: the byte-matching asm, and a FABLE_PORTABLE C++ twin); this row carries no asm of its own.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E2DA0, 79 bytes (`ret 8`). Ego_r.pdb ?SetWobbleParameters@CTCWobble@@QAEXMM@Z.
// Layout (Ego_r.pdb CTCWobble, 48 bytes; the landed 0x007E2F00 model): FramesOfWobbleRemaining +0x24, DirectionLastRotated
// +0x28, AngleOfWobble +0x2c. Global 0x01375550 = frames per second (long).
#pragma optimize("y", off)
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};
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
#include "engine/GFFloatToLongCeil.h"
void CTCWobble::SetWobbleParameters(float seconds, float angle)
{
    AngleOfWobble = angle;
    FramesOfWobbleRemaining = GFFloatToLongCeil(CMainGameComponent::GetConstantFPS() * seconds);
}