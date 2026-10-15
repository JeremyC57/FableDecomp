// C3DAnimation2::ConvertTimeToFrames(float time, long& frame, float& fraction) const -- whole frame (wrapped into the frame count) and the fraction left over
// Grade: DIFFER(115v123) -- lane gap: inline-asm floor helper (fsub 0.5 ; fistp ; fild ; fadd 1.0 with frame-local constants); twin of 0x00A52650 on a record with rate +4 and a ushort frame count +8
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A526D0, 123 bytes, no calls. Layout (C3DAnimationSequenceData-style record):
// samples per second +4, frame count +8 (unsigned short, signed idiv). No caller found (xref 0).
// The original floors through an INLINE-ASM helper (frame pointer, the 0.5f/1.0f constants stored as frame
// locals, `fsub 0.5 ; fistp` then `fild ; fadd 1.0` and an integer compare of the float bits to bump the
// result) -- `fistp` has no compiler-emitted C++ form, so this file only reproduces the surrounding logic.
struct C3DAnimation2 {
    void* __vftable_or_owner;      // +0x00
    float SamplesPerSecond;         // +0x04
    unsigned short FrameCount;      // +0x08
    void ConvertTimeToFrames(float time, long& frame, float& fraction) const;
};

inline long FloorToLong(float value)
{
    float half = 0.5f;
    float one = 1.0f;
    float v = value;
    long result = (long)(v - half);
    float next = (float)result + one;
    if (*(long*)&v == *(long*)&next)
        ++result;
    return result;
}

void C3DAnimation2::ConvertTimeToFrames(float time, long& frame, float& fraction) const
{
    float frames = time * SamplesPerSecond;
    long whole = FloorToLong(frames);
    frame = whole;
    fraction = frames - whole;
    frame = frame % FrameCount;
    if (frame < 0)
        frame += FrameCount;
}
