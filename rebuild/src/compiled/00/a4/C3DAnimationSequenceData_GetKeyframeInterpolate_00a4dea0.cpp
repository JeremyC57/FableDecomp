// C3DAnimationSequenceData::GetKeyframeInterpolate(C3DKeyframe* out, float time) const -- the keyframe at `time`: whole frame (wrapped into the frame count) and the fraction, handed to the interpolating overload
// Leaf: ?GetKeyframeInterpolate@C3DAnimationSequenceData@@QBEXPAVC3DKeyframe@@M@Z
// Retail-asm: 0x00A4DEC1..0x00A4DED0 `fld [ebp-0Ch] ; fsub [ebp-8] ; fistp dword ptr [ebp+0Ch] ; fild [ebp+0Ch] ; fadd [ebp-4] ; fstp [ebp-10h]` -- the engine's inline-assembly floor helper expanded in place (the product homed in the by-value copy, 0.5f / 1.0f stored as frame constants, a bare dword fistp into a long homed in the dead `time` argument slot). VC7.1 emits no bare dword fistp from C++ (`(long)x` calls __ftol2, /QIfist gives `fistp qword`, /arch:SSE2 cvttss2si: rasm1 probe family (a)); the fix-up `mov eax,value ; cmp ; jne ; inc dword ptr [ebp+0Ch]` is part of the same block (10 instructions, one jne: rasm1 family (b)), the result read back by C++.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A4DEA0, 108 bytes (`ret 8`). Callee: the (out, frame, fraction) overload
// 0x00A4C1F0 on the same object (`this` still in ecx: the asm names no register). Layout: Ego_r.pdb C3DAnimationSequenceData
// (BoneName +0, SamplesPerSecond +4, FrameCount ushort +8). Signed modulo by the zero-extended count, biased up when negative.
#pragma optimize("y", off)
class C3DKeyframe;
class C3DAnimationSequenceData { public:                  // Ego_r.pdb (44 bytes)
    char*          BoneName;                        // +0x00
    float          SamplesPerSecond;                // +0x04
    unsigned short FrameCount;                      // +0x08
    void GetKeyframeInterpolate(C3DKeyframe* out, float time) const;                    // 0x00A4DEA0
    void GetKeyframeInterpolate(C3DKeyframe* out, long frame, float fraction) const;    // 0x00A4C1F0
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
void C3DAnimationSequenceData::GetKeyframeInterpolate(C3DKeyframe* out, float time) const
{
    float frames = time * SamplesPerSecond;
    long whole = FloorToLong(frames);
    float fraction = frames - whole;
    long frame = whole % FrameCount;
    if (frame < 0)
        frame += FrameCount;
    GetKeyframeInterpolate(out, frame, fraction);
}