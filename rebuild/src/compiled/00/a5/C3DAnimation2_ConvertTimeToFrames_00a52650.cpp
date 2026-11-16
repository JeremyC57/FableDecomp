// C3DAnimation2::ConvertTimeToFrames(float time, long& frame, float& fraction) const -- whole frame (wrapped into the frame count) and the fraction left over
// Leaf: ?ConvertTimeToFrames@C3DAnimation2@@QBEXMAAJAAM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A52650.
// Retail-asm: 0x00A52671..0x00A52680 `fld [ebp-0Ch] ; fsub [ebp-8] ; fistp dword ptr [ebp+8] ; fild [ebp+8] ; fadd [ebp-4] ; fstp [ebp-10h]` -- the engine's inline-assembly floor helper expanded in place (the product homed in the by-value copy, 0.5f / 1.0f stored as frame constants, a bare dword fistp into a long homed in the dead `time` argument slot). VC7.1 emits no bare dword fistp from C++ (`(long)x` calls __ftol2, /QIfist gives `fistp qword`, /arch:SSE2 cvttss2si: rasm1 probe family (a)); the fix-up `mov eax,value ; cmp ; jne ; inc dword ptr [ebp+8]` is part of the same block (10 instructions, one jne: rasm1 family (b)), the result read back by C++.
// Helper (2026-11-03 asm twins): the retail inline-asm block is the shared engine/GFFloatToLongFloor.h (two-path: the byte-matching asm, and a FABLE_PORTABLE C++ twin); this row carries no asm of its own.
// +0x54 (engine/C3DAnimation2.h). Unsigned modulo (`div`), then the (signed) remainder re-assigned through the reference in both arms of the sign test (a conditional expression: the redundant store in the non-negative arm).
// NF39 (2026-11-16): moved off its derived stand-in (C3DAnimation2_Methods, leaf `ConvertTimeToFrames`) onto the shared
//   engine/C3DAnimation2.h member (opt-in FABLE_RETAIL_C3DANIMATION2_CALCLOCAL declares ?ConvertTimeToFrames@C3DAnimation2@@QBEXMAAJAAM@Z,
//   retail 0x00A52650; Ego_r.pdb + FableWin.pdb carry it); the body is the header's _CONVERTTIME_BODY text, defined out of line here.
#pragma optimize("y", off)
// Preamble from the landed 0x00A4D920 (CalcLocalBoneTransformationRelative, the same opt-in).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_C3DANIMATION2
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CMOVABLERESOURCEMEMORYPOOL
#define FABLE_RETAIL_CSIMPLEMEMORYPOOL
#define FABLE_RETAIL_C3DANIMATION2_CALCLOCAL
#include "engine/C3DAnimation2.h"
// the engine's inline-assembly FPU floor helper (0x0048D940 is its standalone copy): VC7.1 expands the __forceinline body in
// place; the by-value parameter and the four locals become frame words, which is why the function carries an ebp frame.
#include "engine/GFFloatToLongFloor.h"
void C3DAnimation2::ConvertTimeToFrames(float time, long& frame, float& fraction) const
{
    float frames = time * SamplesPerSecond;
    long whole = GFFloatToLongFloor(frames);
    frame = whole;
    fraction = frames - whole;
    frame = (unsigned long)frame % FrameCount;
    frame = frame < 0 ? frame + FrameCount : frame;
}
