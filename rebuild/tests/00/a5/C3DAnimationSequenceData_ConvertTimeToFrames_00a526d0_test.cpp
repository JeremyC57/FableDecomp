#include <cstdio>
#ifdef _MSC_VER
#pragma optimize("y", off)
__forceinline long FloorToLong(float value)
{
    long result; float one; float half; float next;
    half = 0.5f; one = 1.0f;
    __asm {
        fld value
        fsub half
        fistp result
        fild result
        fadd one
        fstp next
        mov eax, value
        cmp eax, next
        jne done
        inc result
    done:
    }
    return result;
}
#else
#include <math.h>
inline long FloorToLong(float value) { return (long)floor((double)value); }   // the FPU block is floor() for the magnitudes used here
#endif
class C3DAnimationSequenceData { public:
    char* BoneName; float SamplesPerSecond; unsigned short FrameCount;
    void ConvertTimeToFrames(float time, long& frame, float& fraction) const
    {
        float frames = time * SamplesPerSecond;
        long whole = FloorToLong(frames);
        frame = whole; fraction = frames - whole;
        frame = frame % FrameCount;
        frame = frame < 0 ? frame + FrameCount : frame;
    }
};
int main()
{
    C3DAnimationSequenceData a; a.BoneName = 0; a.SamplesPerSecond = 30.0f; a.FrameCount = 8;
    long frame = -1; float frac = -1.0f;
    a.ConvertTimeToFrames(0.35f, frame, frac);          // 10.5 -> 2, 0.5
    bool ok = frame == 2 && frac > 0.49f && frac < 0.51f;
    a.ConvertTimeToFrames(-0.05f, frame, frac);         // -1.5 -> floor -2 -> -2 % 8 = -2 -> 6, 0.5
    ok = ok && frame == 6 && frac > 0.49f && frac < 0.51f;
    std::printf(ok ? "00a526d0_TEST PASS\n" : "00a526d0_TEST FAIL\n");
    return ok ? 0 : 1;
}