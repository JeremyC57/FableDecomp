// behaviour test for 0x006E2E60 CTCTalk::StopCurrentSpeech()
#include <cstdio>
struct Player { unsigned int stopped; int calls; void StopAttachedSound(unsigned int s) { stopped = s; ++calls; } };
struct Talk {
    int SpeechSample, SampleStartedFrame, SampleDurationFrames; float MimeStartTime, SampleLoadedTime; unsigned int SampleID; bool LipSynching;
};
static int g_frame = 100;
static void Stop(Talk& t, Player* p, bool has) {
    if (!has) return;
    p->StopAttachedSound(t.SampleID);
    t.SampleDurationFrames = g_frame - t.SampleStartedFrame - 1;
    t.LipSynching = false; t.SpeechSample = 0; t.MimeStartTime = 0.0f; t.SampleLoadedTime = -1.0f; t.SampleID = 0;
}
int main() {
    Player p = {0, 0};
    Talk t = {5, 40, 0, 3.0f, 2.0f, 77, true};
    Stop(t, &p, false);
    if (p.calls != 0 || t.SampleID != 77) { std::printf("006e2e60_TEST FAIL\n"); return 1; }
    Stop(t, &p, true);
    if (p.calls != 1 || p.stopped != 77 || t.SampleDurationFrames != 59 || t.LipSynching || t.SpeechSample != 0 ||
        t.MimeStartTime != 0.0f || t.SampleLoadedTime != -1.0f || t.SampleID != 0) { std::printf("006e2e60_TEST FAIL\n"); return 1; }
    std::printf("006e2e60_TEST PASS\n");
    return 0;
}