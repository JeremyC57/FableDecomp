// C3DAnimationSequenceData::GetKeyframeInterpolate(C3DKeyframe* out, float time) const -- the keyframe at `time`: whole frame (wrapped into the frame count) and the fraction, handed to the interpolating overload
// Leaf: ?GetKeyframeInterpolate@C3DAnimationSequenceData@@QBEXAAVC3DKeyframe@@M@Z
// Retail-asm: 0x00A4DEC1..0x00A4DED0 `fld [ebp-0Ch] ; fsub [ebp-8] ; fistp dword ptr [ebp+0Ch] ; fild [ebp+0Ch] ; fadd [ebp-4] ; fstp [ebp-10h]` -- the engine's inline-assembly floor helper expanded in place (the product homed in the by-value copy, 0.5f / 1.0f stored as frame constants, a bare dword fistp into a long homed in the dead `time` argument slot). VC7.1 emits no bare dword fistp from C++ (`(long)x` calls __ftol2, /QIfist gives `fistp qword`, /arch:SSE2 cvttss2si: rasm1 probe family (a)); the fix-up `mov eax,value ; cmp ; jne ; inc dword ptr [ebp+0Ch]` is part of the same block (10 instructions, one jne: rasm1 family (b)), the result read back by C++.
// Helper (2026-11-03 asm twins): the retail inline-asm block is the shared engine/GFFloatToLongFloor.h (two-path: the byte-matching asm, and a FABLE_PORTABLE C++ twin); this row carries no asm of its own.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A4DEA0, 108 bytes (`ret 8`). Callee: the (out, frame, fraction) overload
// 0x00A4C1F0 on the same object (`this` still in ecx: the asm names no register). Layout: Ego_r.pdb C3DAnimationSequenceData
// (BoneName +0, SamplesPerSecond +4, FrameCount ushort +8). Signed modulo by the zero-extended count, biased up when negative.
#pragma optimize("y", off)
// NF37: moved onto the shared engine/C3DAnimationSequenceData.h (Ego_r.pdb / FableWin.pdb ?GetKeyframeInterpolate@C3DAnimationSequenceData@@QBEXAAVC3DKeyframe@@M@Z
// for this row; the (keyframe, frame, fraction) callee 0x00A4C1F0 is ?GetKeyframeInterpolate@C3DAnimationSequenceData@@QBEXAAVC3DKeyframe@@KM@Z).
#define FABLE_RETAIL_C3DANIMATIONSEQUENCEDATA_INTERPOLATE
#define FABLE_RETAIL_C3DANIMATIONSEQUENCE_GETKEYFRAME
#include "engine/C3DAnimationSequenceData.h"
// the engine's inline-assembly FPU floor helper (0x0048D940 is its standalone copy): VC7.1 expands the __forceinline body in
// place; the by-value parameter and the four locals become frame words, which is why the function carries an ebp frame.
// The whole 10-instruction block is asm (the bit compare and the memory increment included: rasm1 family (b)); the result
// is read next by C++ after the block.
#include "engine/GFFloatToLongFloor.h"
void C3DAnimationSequenceData::GetKeyframeInterpolate(C3DKeyframe& out, float time) const
{
    float frames = time * SamplesPerSecond;
    long whole = GFFloatToLongFloor(frames);
    float fraction = frames - whole;
    long frame = whole % FrameCount;
    if (frame < 0)
        frame += FrameCount;
    GetKeyframeInterpolate(out, frame, fraction);
}