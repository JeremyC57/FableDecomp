// `anonymous namespace'::GFGetGraphicFrame(float scale, unsigned long frameCount, unsigned long firstFrame, unsigned long frame) -- scale a sprite frame's offset from the first frame (wrapping round the frame count) and map the rounded result back into [0, frameCount)
// Grade: RELOCATION_MATCH (2 grades: an external keep-pointer replaces the parked call wrapper) -- rasm1 retail_asm: C++ body + a minimal 2-instruction __asm (no control flow) + the Retail-asm evidence line
// Retail-asm: 0x00643BC9 `fstp [ebp+0Ch]` / 0x00643BCC `fld dword ptr [ebp+0Ch]` + 0x00643BCF `fistp dword ptr [ebp+8]` -- the engine's inline-assembly ROUND helper (the product stored to its by-value copy in the dead `frame` slot and reloaded, a bare dword fistp into the dead `scale` slot). VC7.1 C++ `(long)x` calls __ftol2 and /QIfist (RTM 3077 and QFE 4035, with /Op, /Og or /G5-/G7) emits `fistp qword` into an 8-byte slot, /arch:SSE2 a cvttss2si: no C/C++ form gives the bare dword fistp, so it is the original's `__asm`.
// Leaf: ?GFGetGraphicFrame@?A
// Retail: Fable.exe @ 0x00643B9F, 67 bytes, `ret 8` (byte twin of 0x005BC3ED: the same anonymous-namespace helper in a second TU). Ego_r.pdb: `?GFGetGraphicFrame@?A0x9755b541@@YIKMKKK@Z` (anonymous
// namespace, __fastcall, unsigned long (float, unsigned long, unsigned long, unsigned long)): the two integral leading
// arguments go to ecx (frameCount) / edx (firstFrame), the float and the last word to the stack. No direct callers
// (xref finds none: reached through an inlined/indirect use in the sprite TU).
// Body: offset = frame >= firstFrame ? frame - firstFrame : frameCount - firstFrame + frame;
//       result = Round(scale * (float)offset) + firstFrame; if (result >= frameCount) result -= frameCount;
//   `fild ; jge ; fadd [2^32]` is the unsigned -> float conversion, `fmul [ebp+8]` the scale.
#pragma optimize("y", off)
#pragma optimize("s", on)
// the engine's inline-assembly ROUND helper (round to nearest, the FPU default); expanded in place as a __forceinline
__forceinline long ROUND(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}
namespace {
unsigned long __fastcall GFGetGraphicFrame(float scale, unsigned long frameCount, unsigned long firstFrame, unsigned long frame)
{
    unsigned long offset;
    if (frame >= firstFrame)
        offset = frame - firstFrame;
    else
        offset = frameCount - firstFrame + frame;
    unsigned long result = ROUND(scale * (float)offset) + firstFrame;
    if (result >= frameCount)
        result -= frameCount;
    return result;
}
}
// keeps the internal-linkage body emitted (the retail copy has no direct caller); not part of the retail function
unsigned long (__fastcall *g_keepGFGetGraphicFrame_00643B9F)(float, unsigned long, unsigned long, unsigned long) = &GFGetGraphicFrame;