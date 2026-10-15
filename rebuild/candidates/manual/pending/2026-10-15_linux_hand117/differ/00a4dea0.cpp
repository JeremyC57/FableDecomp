// C3DAnimationSequenceData::GetKeyframeInterpolate(C3DKeyframe* out, float time) const -- the keyframe at `time`: whole frame (wrapped into the frame count) and the fraction, handed to the interpolating getter
// Grade: DIFFER(111v108) -- lane gap: inline-asm floor helper (fsub 0.5 ; fistp ; fild ; fadd 1.0 with frame-local constants, frame pointer), same as 0x00A52650/0x00A526D0
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A4DEA0, 112 bytes. Callee: the keyframe interpolation 0x00A4C1F0
// (out, frame, fraction; no `add esp` = __stdcall-style or a static with callee cleanup).
// Layout: samples per second +4, frame count +8 (unsigned short, signed idiv).
// The original floors through an INLINE-ASM helper (frame pointer, 0.5f/1.0f stored as frame locals,
// `fsub 0.5 ; fistp` then `fild ; fadd 1.0` and an integer compare of the float bits) -- the bare `fistp` has
// no compiler-emitted C++ form, so this file only reproduces the surrounding logic (twin of 0x00A526D0).
struct C3DKeyframe;
void __stdcall InterpolateKeyframe(C3DKeyframe* out, long frame, float fraction);   // 0x00A4C1F0

struct C3DAnimationSequenceData {
    void* m_pOwner;                 // +0x00
    float SamplesPerSecond;         // +0x04
    unsigned short FrameCount;      // +0x08
    void GetKeyframeInterpolate(C3DKeyframe* out, float time) const;
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

void C3DAnimationSequenceData::GetKeyframeInterpolate(C3DKeyframe* out, float time) const
{
    float frames = time * SamplesPerSecond;
    long whole = FloorToLong(frames);
    float fraction = frames - whole;
    long frame = whole % FrameCount;
    if (frame < 0)
        frame += FrameCount;
    InterpolateKeyframe(out, frame, fraction);
}
