// C3DAnimationSequenceData::ConvertTimeToFrames(float time, long& frame, float& fraction) const -- whole frame (wrapped into the sequence's frame count) and the fraction left over
// Leaf: ConvertTimeToFrames
// Retail-asm: 0x00A526F2..0x00A52701 `fld [ebp-0Ch] ; fsub [ebp-8] ; fistp dword ptr [ebp+8] ; fild [ebp+8] ; fadd [ebp-4] ; fstp [ebp-10h]` -- the engine's inline-assembly floor helper expanded in place (the product homed in the by-value copy, 0.5f / 1.0f stored as frame constants, a bare dword fistp into a long homed in the dead `time` argument slot). VC7.1 emits no bare dword fistp from C++ (`(long)x` calls __ftol2, /QIfist gives `fistp qword`, /arch:SSE2 cvttss2si: rasm1 probe family (a)); the fix-up `mov eax,value ; cmp ; jne ; inc dword ptr [ebp+8]` is part of the same block (10 instructions, one jne: rasm1 family (b)), the result read back by C++.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A526D0, 123 bytes (`ret 0Ch`), no calls. The manifest label "C3DAnimation2::
// ConvertTimeToFrames" is propagated from 0x00A52650: this `this` reads SamplesPerSecond at +4 and a 16-bit FrameCount at +8,
// the Ego_r.pdb C3DAnimationSequenceData layout (BoneName +0, SamplesPerSecond +4, FrameCount ushort +8), as 0x00A4DEA0.
// Signed modulo (`cdq ; idiv` by the zero-extended count), the remainder re-assigned through the reference in both arms of the sign test (a conditional expression: the redundant store in the non-negative arm).
#pragma optimize("y", off)
class C3DAnimationSequenceData { public:                  // Ego_r.pdb (44 bytes)
    char*          BoneName;                        // +0x00
    float          SamplesPerSecond;                // +0x04
    unsigned short FrameCount;                      // +0x08
    void ConvertTimeToFrames(float time, long& frame, float& fraction) const;
};
// the engine's inline-assembly FPU floor helper (0x0048D940 is its standalone copy): VC7.1 expands the __forceinline body in
// place; the by-value parameter and the four locals become frame words, which is why the function carries an ebp frame.
// The whole 10-instruction block is asm (the bit compare and the memory increment included: rasm1 family (b)); the result
// is read next by C++ after the block.
__forceinline long FloorToLong(float value)
{
    long  result;
    float one;
    float half;
    float next;
    half = 0.5f;
    one = 1.0f;
    __asm {
        fld   value
        fsub  half
        fistp result
        fild  result
        fadd  one
        fstp  next
        mov   eax, value
        cmp   eax, next
        jne   done
        inc   result
    done:
    }
    return result;
}
void C3DAnimationSequenceData::ConvertTimeToFrames(float time, long& frame, float& fraction) const
{
    float frames = time * SamplesPerSecond;
    long whole = FloorToLong(frames);
    frame = whole;
    fraction = frames - whole;
    frame = frame % FrameCount;
    frame = frame < 0 ? frame + FrameCount : frame;
}