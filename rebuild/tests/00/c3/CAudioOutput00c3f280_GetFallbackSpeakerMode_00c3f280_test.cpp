// Behaviour test for 00c3f280: CAudioOutput::GetFallbackSpeakerMode -- the next simpler speaker configuration for a mode
#include <cstdio>

struct CAudioOutput {
    long GetFallbackSpeakerMode(long mode)
    {
        switch (mode) {
        case 1:
        case 2:
        case 4:
            return 0;
        case 8:
            return 4;
        case 64:
            return 8;
        case 128:
            return 64;
        default:
            return 128;
        }
    }
};

int main()
{
    static const long modes[9] = { 1, 2, 4, 8, 64, 128, 0, 3, 256 };
    static const long want[9] = { 0, 0, 0, 4, 8, 64, 128, 128, 128 };
    CAudioOutput out;
    for (int i = 0; i < 9; ++i)
        if (out.GetFallbackSpeakerMode(modes[i]) != want[i]) { std::printf("00c3f280_TEST FAIL\n"); return 1; }
    std::printf("00c3f280_TEST PASS\n");
    return 0;
}