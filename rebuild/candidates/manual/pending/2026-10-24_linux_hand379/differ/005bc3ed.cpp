// `anonymous namespace'::GFGetGraphicFrame(float scale, unsigned long frameCount, unsigned long firstFrame, unsigned long frame) -- scale a sprite frame's offset from the first frame (wrapping round the frame count) and map the rounded result back into [0, frameCount)
// Grade: DIFFER(66v67) -- retail rounds with a bare `fld [ebp+0Ch] ; fistp dword ptr [ebp+8]` (an inline-asm float->int helper: the float spilled to the dead `frame` slot, the int to the dead `scale` slot); C++ gives `__ftol2` (or `fistp qword` under /QIfist)
// Leaf: GFGetGraphicFrame
// Retail: Fable.exe @ 0x005BC3ED, 67 bytes, `ret 8`. Ego_r.pdb: `?GFGetGraphicFrame@?A0x9755b541@@YIKMKKK@Z` (anonymous
// namespace, __fastcall, unsigned long (float, unsigned long, unsigned long, unsigned long)): the two integral leading
// arguments go to ecx (frameCount) / edx (firstFrame), the float and the last word to the stack. No direct callers
// (xref finds none: reached through an inlined/indirect use in the sprite TU).
// Body: offset = frame >= firstFrame ? frame - firstFrame : frameCount - firstFrame + frame;
//       result = Round(scale * (float)offset) + firstFrame; if (result >= frameCount) result -= frameCount;
//   `fild ; jge ; fadd [2^32]` is the unsigned -> float conversion, `fmul [ebp+8]` the scale.
// Lane gap: the rounding is the retail inline-asm idiom (the CLAUDE.md `retail_asm` exception names bare `fistp dword`);
// this lane forbids `__asm`, so the row is parked with the pure C++ form.
#pragma optimize("y", off)
#pragma optimize("s", on)
namespace {
unsigned long __fastcall GFGetGraphicFrame(float scale, unsigned long frameCount, unsigned long firstFrame, unsigned long frame)
{
    unsigned long offset;
    if (frame >= firstFrame)
        offset = frame - firstFrame;
    else
        offset = frameCount - firstFrame + frame;
    float scaled = scale * (float)offset;
    unsigned long result = (long)scaled + firstFrame;
    if (result >= frameCount)
        result -= frameCount;
    return result;
}
}
unsigned long __fastcall CallGFGetGraphicFrame(float scale, unsigned long frameCount, unsigned long firstFrame, unsigned long frame)
{
    return GFGetGraphicFrame(scale, frameCount, firstFrame, frame);
}
