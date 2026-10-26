#include <cstdio>
// Behaviour test for 00681680 COpinionTransientOffset ctor: peak and persist scaled by the sign-specific traits, frame counts
// time-scaled and truncated (zero run-in/run-out become one frame), slopes rebuilt from the scaled values.
class COpinionPersonalityTraits { public:                 // 36 bytes
    float BaseOffset;                              // +0x00
    float HeroStatScaling;                         // +0x04
    float VillageOpinionScaling;                   // +0x08
    float TimeScalingRunIn;                        // +0x0c
    float TimeScalingRunOut;                       // +0x10
    float PeakScalingIfPos;                        // +0x14
    float PeakScalingIfNeg;                        // +0x18
    float PersistScalingIfPos;                     // +0x1c
    float PersistScalingIfNeg;                     // +0x20
};
typedef long EOpinion;
class COpinionTransientOffset {
public:
    EOpinion Opinion;                              // +0x00
    float    OffsetPerFrameRunIn;                  // +0x04
    long     FramesToRunIn;                        // +0x08
    long     FramesOfCappedPeak;                   // +0x0c
    float    OffsetPerFrameRunOut;                 // +0x10
    long     FramesToRunOut;                       // +0x14
    COpinionTransientOffset(const COpinionTransientOffset& base, const COpinionPersonalityTraits& traits);
};

COpinionTransientOffset::COpinionTransientOffset(const COpinionTransientOffset& base, const COpinionPersonalityTraits& traits)
    : Opinion(base.Opinion)
{
    float runIn = (float)base.FramesToRunIn;
    float peak = runIn * base.OffsetPerFrameRunIn;
    float persist = (float)base.FramesToRunOut * base.OffsetPerFrameRunOut + peak;
    peak *= (peak >= 0.0f) ? traits.PeakScalingIfPos : traits.PeakScalingIfNeg;
    persist *= (persist >= 0.0f) ? traits.PersistScalingIfPos : traits.PersistScalingIfNeg;
    FramesToRunIn = (long)(runIn * traits.TimeScalingRunIn);
    FramesOfCappedPeak = (long)((float)base.FramesOfCappedPeak * traits.TimeScalingRunOut);
    FramesToRunOut = (long)((float)base.FramesToRunOut * traits.TimeScalingRunOut);
    long runInFrames = FramesToRunIn ? FramesToRunIn : 1;
    FramesToRunIn = runInFrames;
    long runOutFrames = FramesToRunOut ? FramesToRunOut : 1;
    FramesToRunOut = runOutFrames;
    OffsetPerFrameRunIn = peak / runInFrames;
    OffsetPerFrameRunOut = (persist - peak) / runOutFrames;
}

static COpinionTransientOffset& SeedOffset() { static char raw[sizeof(COpinionTransientOffset)] = { 0 }; return *(COpinionTransientOffset*)raw; }
static bool Near(float a, float b) { float d = a - b; return d < 1e-4f && d > -1e-4f; }
int main()
{
    COpinionPersonalityTraits t = { 0, 0, 0, 2.0f, 0.5f, 3.0f, 0.25f, 4.0f, 0.1f };
    COpinionTransientOffset base = COpinionTransientOffset(SeedOffset(), t);
    base.Opinion = 6; base.OffsetPerFrameRunIn = 1.5f; base.FramesToRunIn = 4; base.FramesOfCappedPeak = 9;
    base.OffsetPerFrameRunOut = -0.5f; base.FramesToRunOut = 6;
    // peak = 6, persist = 6 - 3 = 3 (both positive): peak*3 = 18, persist*4 = 12; frames 8, 4 (4.5 truncated), 3
    COpinionTransientOffset a(base, t);
    bool ok = a.Opinion == 6 && a.FramesToRunIn == 8 && a.FramesOfCappedPeak == 4 && a.FramesToRunOut == 3;
    ok = ok && Near(a.OffsetPerFrameRunIn, 18.0f / 8.0f) && Near(a.OffsetPerFrameRunOut, (12.0f - 18.0f) / 3.0f);
    // negative peak, negative persist, zero frames clamp to one
    base.OffsetPerFrameRunIn = -2.0f; base.FramesToRunIn = 1; base.OffsetPerFrameRunOut = -1.0f; base.FramesToRunOut = 1;
    COpinionPersonalityTraits z = { 0, 0, 0, 0.1f, 0.1f, 3.0f, 0.25f, 4.0f, 0.1f };
    COpinionTransientOffset b(base, z);           // peak -2 * 0.25 = -0.5, persist -3 * 0.1 = -0.3; frames 0 -> 1
    ok = ok && b.FramesToRunIn == 1 && b.FramesToRunOut == 1 && b.FramesOfCappedPeak == 0;
    ok = ok && Near(b.OffsetPerFrameRunIn, -0.5f) && Near(b.OffsetPerFrameRunOut, -0.3f + 0.5f);
    if (!ok) { std::printf("00681680_TEST FAIL\n"); return 1; }
    std::printf("00681680_TEST PASS\n");
    return 0;
}