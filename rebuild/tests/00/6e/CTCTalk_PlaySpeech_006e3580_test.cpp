// Behaviour test for CTCTalk::PlaySpeech 0x006E3580 (model copy): no sound player -> FAILED_FINAL; a running speech is stopped;
// a narrator outside the permitted AI narrator list clears IsPureAISpeaker; the narrator is remembered; the sample handle is stored;
// the gain manager hears of the dialogue only for a valid handle; the voice pitch override is applied when != 1.0; the frame window restarts.
#include <stdio.h>
#include <string.h>
#include <vector>
#include <algorithm>
#include <string>
static long g_frame = 100; static unsigned long g_invalid = 0xFFFFFFFF;
static int g_stopped, g_dialogue, g_pitch_set, g_lip; static float g_pitch;
struct Player { unsigned long next; unsigned long Play(long s) { return next; } };
struct Talk {
    Player* player; long SampleStartedFrame, SampleDurationFrames; unsigned long SampleID; std::string LastNarrator;
    bool IsPureAISpeaker, UseLipSynching; float SampleLoadedTime, VoicePitch; std::vector<std::string>* permitted;
    bool IsTalking() const { return g_frame - SampleStartedFrame < SampleDurationFrames; }
    void SetAsUsingNarrator(const std::string& n) {
        if (IsPureAISpeaker) { std::vector<std::string>::const_iterator last = permitted->end();
            const std::vector<std::string>& v = *permitted; if (std::find(v.begin(), last, n) == last) IsPureAISpeaker = false; }
    }
    int PlaySpeech(long sample, const std::string& narrator, bool c) {
        if (!player) return 1;                                   // SPEECH_PLAYBACK_FAILED_FINAL (retail 0x006E3773 mov eax,1)
        if (IsTalking()) ++g_stopped;                            // StopCurrentSpeech
        SetAsUsingNarrator(narrator);
        LastNarrator = narrator;
        SampleID = player->Play(sample);
        if (SampleID != g_invalid) ++g_dialogue;                 // OnDialogueStart
        if (VoicePitch != 1.0f) { ++g_pitch_set; g_pitch = VoicePitch; }
        UseLipSynching = c; ++g_lip;                             // PlayLipMovement
        SampleLoadedTime = -1.0f;
        SampleStartedFrame = g_frame;
        SampleDurationFrames = 50 + sample;                      // GetSpeechDurationInFrames
        return 0;                                                // SPEECH_PLAYBACK_STARTED
    }
};
int main() {
    std::vector<std::string> permitted; permitted.push_back("Narrator"); permitted.push_back("Guild");
    Player p; p.next = 7;
    Talk t; t.player = 0; t.SampleStartedFrame = 0; t.SampleDurationFrames = 0; t.SampleID = 0; t.IsPureAISpeaker = true;
    t.UseLipSynching = false; t.SampleLoadedTime = 0; t.VoicePitch = 1.0f; t.permitted = &permitted;
    int ok = 1;
    if (t.PlaySpeech(3, "Guild", true) != 1 || g_lip != 0) ok = 0;
    t.player = &p;
    if (t.PlaySpeech(3, "Guild", true) != 0 || g_stopped != 0 || !t.IsPureAISpeaker || t.LastNarrator != "Guild" || t.SampleID != 7) ok = 0;
    if (g_dialogue != 1 || g_pitch_set != 0 || !t.UseLipSynching || t.SampleStartedFrame != 100 || t.SampleDurationFrames != 53) ok = 0;
    p.next = g_invalid; t.VoicePitch = 1.5f; g_frame = 120;              // still talking (120-100 < 53)
    if (t.PlaySpeech(4, "Hero", false) != 0 || g_stopped != 1 || t.IsPureAISpeaker || t.LastNarrator != "Hero") ok = 0;
    if (g_dialogue != 1 || g_pitch_set != 1 || g_pitch != 1.5f || t.UseLipSynching || t.SampleLoadedTime != -1.0f) ok = 0;
    if (ok) { printf("006e3580_TEST PASS\n"); return 0; }
    printf("006e3580_TEST FAIL\n"); return 1;
}