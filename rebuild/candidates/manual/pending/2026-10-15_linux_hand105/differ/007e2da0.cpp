// CTCWobble::SetWobbleParameters(float, float) -- store the wobble angle and the wobble duration in frames (seconds * frames-per-second, rounded up)
// Grade: DIFFER(92v79) -- lane gap: retail inlines an inline-assembly float-to-int ceiling helper (`fld f ; fadd half ;
// fistp dword [n] ; fild [n] ; fsub one ; fstp t ; mov eax,f ; cmp eax,t ; jne ; dec n` with every operand in the frame,
// the 0.5f / 1.0f operands stored to locals). Plain C++ cannot emit `fistp dword` (VC7.1 /QIfist gives `fistp qword`) nor
// keep the operands in memory this way; only an asm_bake oracle reproduces it.
// Flags tried: --flags /QIfist /Op (this source).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E2DA0, length 79 bytes (+ int3 padding).
// Ego_r.pdb: ?SetWobbleParameters@CTCWobble@@QAEXMM@Z. Layout (Ego_r.pdb CTCWobble, 48 bytes): CTCBase 0xc, ParentRHSet
// +0x0c, FramesOfWobbleRemaining +0x24, DirectionLastRotated +0x28, AngleOfWobble +0x2c. Global 0x01375550 = frames per
// second (long, 15).
// Shape (retail): frame-pointer body (the inline-asm helper forces ebp); the angle copied as a dword; `fild [fps] ;
// fmul [seconds]` into a frame local; the rounded count homed in the dead `angle` slot and 1.0f in the dead `seconds`
// slot; `n = round(f + 0.5) ; if (n - 1 == f) --n` (bitwise compare) = ceil(f).
#pragma optimize("y", off)
extern long g_FramesPerSecond;                              // 0x01375550

inline long CeilFloatToLong(float value)
{
    volatile float half = 0.5f;
    volatile float one = 1.0f;
    long result = (long)(value + half);
    float back = (float)result - one;
    if (*(long*)&value == *(long*)&back)
        --result;
    return result;
}

struct CTCWobble {
    void*         __vftable;                                // +0x00
    void*         ParentThing;                              // +0x04
    unsigned char _pad_0x08[0x24 - 0x08];
    long          FramesOfWobbleRemaining;                  // +0x24
    float         DirectionLastRotated;                     // +0x28
    float         AngleOfWobble;                            // +0x2c
    void SetWobbleParameters(float seconds, float angle);
};

void CTCWobble::SetWobbleParameters(float seconds, float angle)
{
    AngleOfWobble = angle;
    FramesOfWobbleRemaining = CeilFloatToLong(g_FramesPerSecond * seconds);
}
