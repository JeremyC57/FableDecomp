// CThingPhysical::ModifyHealth(float amount, bool fromHit) -- rounds the health change to the nearest whole number (ties away from zero) and applies it through InternalModifyHealth when it is not (near) zero
// 0.5f/1.0f frame constants and an exact-tie fix-up `cmp eax,[ebp-10h] ; jne ; inc/dec [ebp+8]`); the lane compiles (long) casts to
// _ftol calls (known lane gap: bare dword fistp, see the PackVertexNormal family)
// Leaf: ?ModifyHealth@CThingPhysical
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A68F0, length 159 bytes (`ret 8`), frame pointer (`optimize("y", off)` TU).
// Callees: CThingPhysical::InternalModifyHealth(float, bool) @ 0x006A62E0.
// Globals: float 0.0f @ 0x0122DEDC, float 1e-4f @ 0x0129BA3C.
// Retail-asm: 0x006A6947..0x006A6961 (ceiling, amount >= 0: `fld [ebp-8] ; fadd [ebp-0Ch] ; fistp dword ptr [ebp+8] ; fild [ebp+8] ; fsub [ebp-10h] ; fstp [ebp-4] ; mov eax,[ebp-8] ; cmp eax,[ebp-4] ; jne ; dec dword ptr [ebp+8]`) and 0x006A6917..0x006A6931 (floor, amount < 0: `fld [ebp-0Ch] ; fsub [ebp-8] ; fistp dword ptr [ebp+8] ; fild [ebp+8] ; fadd [ebp-4] ; fstp [ebp-10h] ; mov eax ; cmp ; jne ; inc dword ptr [ebp+8]`), both results homed in the dead `amount` slot -- the engine's inline-assembly ceiling helper expanded in place (0.5f / 1.0f stored as frame constants, a bare dword fistp into a named long, the bit compare and `jne ; dec dword ptr` in the same 10-instruction block: rasm1 family (b)). VC7.1 emits no bare dword fistp from C++ (`(long)x` calls __ftol2, /QIfist gives `fistp qword`, /arch:SSE2 cvttss2si: rasm1 probe family (a)); the result is read back by C++ after the block.
// Helper (2026-11-03 asm twins): the retail inline-asm block is the shared engine/GFFloatToLongCeil.h, engine/GFFloatToLongFloor.h (two-path: the byte-matching asm, and a FABLE_PORTABLE C++ twin); this row carries no asm of its own.
#pragma optimize("y", off)
#include <math.h>
// the engine's inline-assembly FPU ceiling helper (0x004FA9A0 is its standalone copy): VC7.1 expands the __forceinline body in
// place; the by-value parameter and the four locals become frame words, which is why the function carries an ebp frame.
// The whole 10-instruction block is asm (the bit compare and the memory decrement included: rasm1 family (b)); the result
// is read back by C++ after the block.
#include "engine/GFFloatToLongCeil.h"
// the engine's inline-assembly FPU floor helper (0x0048D940 is its standalone copy): VC7.1 expands the __forceinline body in
// place; the by-value parameter and the four locals become frame words, which is why the function carries an ebp frame.
// The whole 10-instruction block is asm (the bit compare and the memory increment included: rasm1 family (b)); the result
// is read next by C++ after the block.
#include "engine/GFFloatToLongFloor.h"
class CThingPhysical { void InternalModifyHealth(float a0, bool a1);   // 0x006A62E0
    void ModifyHealth(float amount, bool fromHit);
};



void CThingPhysical::ModifyHealth(float amount, bool fromHit)
{
    if (amount < 0.0f)
        amount = (float)GFFloatToLongFloor(amount);
    else
        amount = (float)GFFloatToLongCeil(amount);
    if ((float)fabs(amount) > 1e-4f)
        InternalModifyHealth(amount, fromHit);
}