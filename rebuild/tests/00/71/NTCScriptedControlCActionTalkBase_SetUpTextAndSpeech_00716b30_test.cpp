// Behaviour test for 0x00716B30 NTCScriptedControl::CActionTalkBase::SetUpTextAndSpeech()
#include <stdio.h>
#include <vector>
#ifdef _MSC_VER
typedef unsigned __int64 UInt64;
#else
typedef unsigned long long UInt64;
#endif

static long CeilToLong(float v) { long r = (long)v; if ((float)r < v) ++r; return r; }   // GFFloatToLongCeil off the boundaries
static const long ConstantFPS = 30;

class CThing { public: UInt64 UniqueID; };
class CScriptThing { public: CThing* Thing; bool IsAlive() const { return Thing != 0; } CThing* GetPThing() const { return Thing; } };
class CTCTalk { public:
    int Played, LipMoves; long PlayedIndex; bool PlayedIn2D; long Attitude; float Duration;
    void PlaySpeech(long index, bool in2D) { ++Played; PlayedIndex = index; PlayedIn2D = in2D; }
    void PlayRandomLipMovement() { ++LipMoves; }
    float GetCurrentSpeechDuration() const { return Duration; }
    void SetAttitude(long a) { Attitude = a; }
};
class CSubtitledMessage { public:
    long SpeechIndex;
    std::vector<long> AttitudeTags, AnimationTags, CameraTags;
};
static UInt64 SpeakerUniqueID, ListenerUniqueID;
static long CurrentCameraTagIndex = -1, ConversationCameraSide = 5;
static bool g_scripted;
static long g_nextTag;
static int g_display, g_setUpCamera;
static float g_random;

struct CActionTalkBase {
    CThing* Parent; CTCTalk* Talk; CScriptThing PTargetThing;
    long UpdateCounter; double TimeCurrentLineStarted, TimeCurrentLineLasts;
    std::vector<CSubtitledMessage> Messages; long CurrentMessageIndex;
    long GameTurnDuration, NextAttitudeTagIndex, NextAnimationTagIndex, NextCameraTagIndex, FramesUntilNextGesture;
    bool UseCameraTags, CameraTagsWereUsed, SoundIn2D, SoundStartedPlaying, SkippedCurrentLine;
    void SetUpTextAndSpeech()                                   // the row's body (callees stubbed)
    {
        NextAttitudeTagIndex = -1;
        NextAnimationTagIndex = -1;
        NextCameraTagIndex = -1;
        SoundStartedPlaying = false;
        SkippedCurrentLine = false;
        UInt64 speaker = Parent->UniqueID;
        UInt64 listener = 0;
        if (PTargetThing.IsAlive())
            listener = PTargetThing.GetPThing()->UniqueID;
        if (!((SpeakerUniqueID == speaker || SpeakerUniqueID == listener)
              && (ListenerUniqueID == speaker || ListenerUniqueID == listener))) {
            CurrentCameraTagIndex = -1;
            ConversationCameraSide = 0;
        }
        SpeakerUniqueID = speaker;
        ListenerUniqueID = listener;
        CTCTalk* talk = Talk;
        if (talk) {
            const CSubtitledMessage& message = Messages[CurrentMessageIndex];
            if (message.SpeechIndex > 0) {
                talk->PlaySpeech(message.SpeechIndex, SoundIn2D);
            } else {
                talk->PlayRandomLipMovement();
                SoundStartedPlaying = true;
                TimeCurrentLineStarted = 123.0;
            }
            TimeCurrentLineLasts = talk->GetCurrentSpeechDuration() + 0.5f;
            GameTurnDuration = CeilToLong((float)(ConstantFPS * TimeCurrentLineLasts));
            UpdateCounter = 0;
            double gesture_delay = g_random + 0.5f;
            FramesUntilNextGesture = CeilToLong((float)(ConstantFPS * gesture_delay));
            if (!Messages[CurrentMessageIndex].AttitudeTags.empty())
                talk->SetAttitude(Messages[CurrentMessageIndex].AttitudeTags[0]);
            if (!Messages[CurrentMessageIndex].AnimationTags.empty()) {
                NextAnimationTagIndex = 0;
                FramesUntilNextGesture = -1;
            }
            if (UseCameraTags) {
                if (!Messages[CurrentMessageIndex].CameraTags.empty()) {
                    NextCameraTagIndex = 0;
                } else {
                    if (!g_scripted)
                        CameraTagsWereUsed = true;
                    long index = g_nextTag;
                    if (index != CurrentCameraTagIndex) {
                        CurrentCameraTagIndex = index;
                        ++g_setUpCamera;
                    }
                }
            }
        }
        ++g_display;
    }
};

int main()
{
    int ok = 1;
    CThing hero = { 7 }, villager = { 9 };
    CTCTalk talk = { 0, 0, 0, false, 0, 1.01f };
    CActionTalkBase a;
    a.Parent = &villager; a.Talk = &talk; a.PTargetThing.Thing = &hero;
    a.UpdateCounter = 99; a.TimeCurrentLineStarted = 0; a.CurrentMessageIndex = 0;
    a.UseCameraTags = true; a.CameraTagsWereUsed = false; a.SoundIn2D = true; a.SoundStartedPlaying = true; a.SkippedCurrentLine = true;
    CSubtitledMessage m; m.SpeechIndex = 4; m.AttitudeTags.push_back(3);
    a.Messages.push_back(m);
    g_random = 0.21f; g_nextTag = 2; g_scripted = false;
    a.SetUpTextAndSpeech();                                     // new pair: camera side reset, speech played
    ok &= ConversationCameraSide == 0 && SpeakerUniqueID == 9 && ListenerUniqueID == 7;
    ok &= talk.Played == 1 && talk.PlayedIndex == 4 && talk.PlayedIn2D && !a.SoundStartedPlaying && !a.SkippedCurrentLine;
    ok &= a.GameTurnDuration == 46 && a.FramesUntilNextGesture == 22 && a.UpdateCounter == 0;   // 45.3 -> 46, 21.3 -> 22
    ok &= talk.Attitude == 3 && a.NextAttitudeTagIndex == -1 && a.NextAnimationTagIndex == -1;
    ok &= a.CameraTagsWereUsed && CurrentCameraTagIndex == 2 && g_setUpCamera == 1 && g_display == 1;
    ConversationCameraSide = 5;
    a.Messages[0].SpeechIndex = 0; a.Messages[0].AnimationTags.push_back(1); a.Messages[0].CameraTags.push_back(1);
    a.Parent = &hero; a.PTargetThing.Thing = &villager;         // same pair, swapped: no reset
    a.SetUpTextAndSpeech();
    ok &= ConversationCameraSide == 5 && talk.LipMoves == 1 && a.SoundStartedPlaying && a.TimeCurrentLineStarted == 123.0;
    ok &= a.NextAnimationTagIndex == 0 && a.FramesUntilNextGesture == -1 && a.NextCameraTagIndex == 0 && g_setUpCamera == 1;
    a.Talk = 0;                                                 // no talk component: only the text display
    a.SetUpTextAndSpeech();
    ok &= g_display == 3 && a.NextCameraTagIndex == -1;
    printf(ok ? "00716b30_TEST PASS\n" : "00716b30_TEST FAIL\n");
    return ok ? 0 : 1;
}