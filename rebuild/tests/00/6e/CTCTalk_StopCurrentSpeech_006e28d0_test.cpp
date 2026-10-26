// Behaviour test for CTCTalk::StopCurrentSpeech @ 0x006E28D0: stop the sample, cut the window, reset state.
#include <cstdio>
static long g_frame;
struct CSpeechEmitter { unsigned long stopped; void StopSample(unsigned long id) { stopped = id; } };
class CTCTalk { public:
    long SpeechSample, SampleStartedFrame, SampleDurationFrames, Attitude;
    float MimeStartTime, SampleLoadedTime; unsigned long SampleID; bool LipSynching;
    bool IsTalking() { return g_frame - SampleStartedFrame < SampleDurationFrames; }
    void StopCurrentSpeech(CSpeechEmitter* e)
    {
        e->StopSample(SampleID);
        SampleDurationFrames = g_frame - SampleStartedFrame - 1;
        LipSynching = false; SpeechSample = 0; MimeStartTime = 0.0f; SampleLoadedTime = -1.0f; SampleID = 0;
    }
};
int main()
{
    CTCTalk t; CSpeechEmitter e; e.stopped = 0;
    t.SpeechSample = 5; t.SampleStartedFrame = 50; t.SampleDurationFrames = 1000; t.Attitude = 2;
    t.MimeStartTime = 3.0f; t.SampleLoadedTime = 4.0f; t.SampleID = 77; t.LipSynching = true;
    g_frame = 60; t.StopCurrentSpeech(&e);
    if (e.stopped != 77 || t.SampleDurationFrames != 9 || t.IsTalking() || t.LipSynching || t.SpeechSample != 0
        || t.MimeStartTime != 0.0f || t.SampleLoadedTime != -1.0f || t.SampleID != 0 || t.Attitude != 2) { std::printf("006e28d0_TEST FAIL\n"); return 1; }
    std::printf("006e28d0_TEST PASS\n");
    return 0;
}