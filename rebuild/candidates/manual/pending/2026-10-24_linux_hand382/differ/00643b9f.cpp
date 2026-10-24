// GFGetGraphicFrame(float fraction, unsigned long numFrames, unsigned long startFrame, unsigned long frame) -- the animation frame a fraction of the way from startFrame towards frame, wrapping round numFrames
// Leaf: ?GFGetGraphicFrame@
// Grade: DIFFER(66v67) -- asm-only rounding: retail `fstp [ebp+0Ch] ; fld [ebp+0Ch] ; fistp dword [ebp+8]` (an inline-asm fld/fistp helper, frame-pointer body) where C++ gives `call __ftol2` and no frame; the wrap-around arithmetic and the unsigned->float conversion match. Candidate for the retail_asm grade (the brief forbids inline asm).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00643B9F, 67 bytes, `ret 8` (__fastcall: numFrames in ecx, startFrame in edx,
// fraction and frame on the stack). Size-optimised NPlayerGui TU (frame pointer).
// Identity: Ego_r.pdb `?GFGetGraphicFrame@?A0x4f2340bf@@YIKMKKK@Z` (file-local, in an anonymous namespace; a second copy
// `?A0x9755b541` exists in another TU). Modelled outside the anonymous namespace so the unreferenced body is emitted.
// Shape: the forward distance (frame - start, or numFrames - start + frame when frame is behind), converted unsigned ->
// float (`fild ; jge ; fadd 2^32`, pool 0x0122DCB4), scaled by the fraction, rounded to an integer, then start added
// back modulo numFrames. Retail rounds with a bare `fld [f] ; fistp dword [i]` on a float homed in the dead `frame`
// slot: an inline-asm rounding helper in the original (GOTCHAS: dword fistp ROUND helpers are asm-only); C++ gives
// `__ftol2` (truncation) instead.
unsigned long __fastcall GFGetGraphicFrame(float fraction, unsigned long numFrames, unsigned long startFrame, unsigned long frame)
{
    unsigned long distance = (frame >= startFrame) ? frame - startFrame : numFrames - startFrame + frame;
    float scaled = (float)distance * fraction;
    unsigned long result = (long)scaled + startFrame;
    if (result >= numFrames)
        result -= numFrames;
    return result;
}
