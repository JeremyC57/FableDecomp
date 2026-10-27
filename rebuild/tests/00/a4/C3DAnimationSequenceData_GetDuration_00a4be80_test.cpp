// Behaviour test for 0x00A4BE80 C3DAnimationSequenceData::GetDuration(bool) const
#include <cstdio>
class C3DAnimationSequenceData {
public:
    char*          BoneName;
    float          SamplesPerSecond;
    unsigned short FrameCount;
    float GetDuration(bool looping) const
    {
        if (looping)
            return FrameCount * SamplesPerSecond;
        return (FrameCount - 1) * SamplesPerSecond;
    }
};
int main()
{
    C3DAnimationSequenceData d;
    d.BoneName = 0; d.SamplesPerSecond = 0.5f; d.FrameCount = 11;
    if (d.GetDuration(true) != 5.5f) { std::printf("00a4be80_TEST FAIL\n"); return 1; }
    if (d.GetDuration(false) != 5.0f) { std::printf("00a4be80_TEST FAIL\n"); return 1; }
    d.FrameCount = 0;
    if (d.GetDuration(false) != -0.5f) { std::printf("00a4be80_TEST FAIL\n"); return 1; }
    std::printf("00a4be80_TEST PASS\n");
    return 0;
}