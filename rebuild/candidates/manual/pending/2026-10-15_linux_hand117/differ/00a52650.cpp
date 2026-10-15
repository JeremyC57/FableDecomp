// C3DAnimation2::ConvertTimeToFrames(float time, long& frame, float& fraction) const -- whole frame (wrapped into the frame count) and the fraction left over
// Grade: DIFFER(110v117) -- lane gap: inline-asm floor helper (fsub 0.5 ; fistp ; fild ; fadd 1.0 with the constants as frame locals, frame pointer); no compiler-emitted C++ produces the bare fistp
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A52650, 128 bytes, no calls. Retail layout: SamplesPerSecond +0x50,
// FrameCount +0x54 (the PDB header engine/C3DAnimation2.h sits 4 bytes high from +0x4C).
// The original floors through an INLINE-ASM helper (frame pointer, the 0.5f/1.0f constants stored as frame
// locals, `fsub 0.5 ; fistp` then `fild ; fadd 1.0` and an integer compare of the float bits to bump the
// result) -- `fistp` has no compiler-emitted C++ form, so this file only reproduces the surrounding logic.
struct C3DAnimation2 {
    unsigned char _body[0x50];
    float SamplesPerSecond;         // +0x50
    unsigned long FrameCount;       // +0x54
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
    frame = (unsigned long)frame % FrameCount;
    if (frame < 0)
        frame += FrameCount;
}
