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
class C3DKeyframe { public: long frame; float fraction; };
class C3DAnimationSequenceData { public:
    char* BoneName; float SamplesPerSecond; unsigned short FrameCount;
    void GetKeyframeInterpolate(C3DKeyframe* out, long frame, float fraction) const { out->frame = frame; out->fraction = fraction; }
    void GetKeyframeInterpolate(C3DKeyframe* out, float time) const
    {
        float frames = time * SamplesPerSecond;
        long whole = FloorToLong(frames);
        float fraction = frames - whole;
        long frame = whole % FrameCount;
        if (frame < 0) frame += FrameCount;
        GetKeyframeInterpolate(out, frame, fraction);
    }
};
int main()
{
    C3DAnimationSequenceData a; a.BoneName = 0; a.SamplesPerSecond = 30.0f; a.FrameCount = 8;
    C3DKeyframe k; k.frame = -1; k.fraction = -1.0f;
    a.GetKeyframeInterpolate(&k, 0.35f);                 // 10.5 -> 2, 0.5
    bool ok = k.frame == 2 && k.fraction > 0.49f && k.fraction < 0.51f;
    a.GetKeyframeInterpolate(&k, -0.05f);                // -1.5 -> -2 -> 6, 0.5
    ok = ok && k.frame == 6 && k.fraction > 0.49f && k.fraction < 0.51f;
    std::printf(ok ? "00a4dea0_TEST PASS\n" : "00a4dea0_TEST FAIL\n");
    return ok ? 0 : 1;
}