// CTCBulletTime_Methods::GetBulletTimeElapsedTicks() const -- ticks of bullet time used so far: ceil(ticks-per-second * m_fDuration) - BulletTimeRemainingTicks
// Leaf: GetBulletTimeElapsedTicks
// Naming wave 18: the global stand-in g_TicksPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail-asm: 0x0079C6F0..0x0079C6FF `fld [ebp-10h] ; fadd [ebp-0Ch] ; fistp dword ptr [ebp-4] ; fild [ebp-4] ; fsub [ebp-8] ; fstp [ebp-14h]` -- the engine's inline-assembly ceiling helper expanded in place (0.5f / 1.0f stored as frame constants, a bare dword fistp into a named long). VC7.1 emits no bare dword fistp from C++ (`(long)x` calls __ftol2, /QIfist gives `fistp qword`, /arch:SSE2 cvttss2si: rasm1 probe family (a)); the fix-up `mov eax,value ; cmp eax,back ; jne ; dec dword ptr` is part of the same block (10 instructions, one jne: rasm1 family (b)), the result read back by C++.
// Helper (2026-11-03 asm twins): the retail inline-asm block is the shared engine/GFFloatToLongCeil.h (two-path: the byte-matching asm, and a FABLE_PORTABLE C++ twin); this row carries no asm of its own.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0079C6D0, 71 bytes. Globals: 0x01375550 = the game's ticks-per-second long.
// BulletTimeRemainingTicks +0x0c, m_fDuration +0x2c (engine header).
#pragma optimize("y", off)
#include "engine/CTCBulletTime.h"
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};
struct CTCBulletTime_Methods : CTCBulletTime {       // the PDB layout header carries no methods
    long GetBulletTimeElapsedTicks() const;
};
// the engine's inline-assembly FPU ceiling helper (0x004FA9A0 is its standalone copy): VC7.1 expands the __forceinline body in
// place; the by-value parameter and the four locals become frame words, which is why the function carries an ebp frame.
// The whole 10-instruction block is asm (the bit compare and the memory decrement included: rasm1 family (b)); the result
// is read back by C++ after the block.
#include "engine/GFFloatToLongCeil.h"
long CTCBulletTime_Methods::GetBulletTimeElapsedTicks() const
{
    return GFFloatToLongCeil(CMainGameComponent::GetConstantFPS() * m_fDuration) - BulletTimeRemainingTicks;
}