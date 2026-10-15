// Behaviour test for CTCTalk::IsTalking @ 0x006E28A0: frame - started < duration (signed).
#include <cstdio>
static long g_frame;
struct CTCTalk {
    long SampleStartedFrame, SampleDurationFrames;
    bool IsTalking() { return g_frame - SampleStartedFrame < SampleDurationFrames; }
};
int main()
{
    CTCTalk t; t.SampleStartedFrame = 100; t.SampleDurationFrames = 30;
    g_frame = 100; if (!t.IsTalking()) { std::printf("006e28a0_TEST FAIL\n"); return 1; }
    g_frame = 129; if (!t.IsTalking()) { std::printf("006e28a0_TEST FAIL\n"); return 1; }
    g_frame = 130; if (t.IsTalking()) { std::printf("006e28a0_TEST FAIL\n"); return 1; }
    t.SampleDurationFrames = -1; g_frame = 100; if (t.IsTalking()) { std::printf("006e28a0_TEST FAIL\n"); return 1; }
    std::printf("006e28a0_TEST PASS\n");
    return 0;
}
