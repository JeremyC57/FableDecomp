// 00714e00 behaviour test: CActionTalkBase::OnCleanup -- a listening target (not the talker) stops looking; the
// talker's attitude is restored unless the conversation mode was added; a running conversation mode lingers
// fps * 3 frames, any other current mode makes the conversation mode be removed; the subtitle background is shown and
// the subtitles are cleared when the profile (or the default when no profile is loaded) has them off.
#include <cstdio>

struct CTCLook { int stopCalls; long lastTime; void StopLooking(long t) { ++stopCalls; lastTime = t; } };
struct CTCTalk { long ConversationAttitude; };
struct CMode { long type; long LingerFrames; long GetType() const { return type; } };
struct CTCCreatureModeManager {
    CMode current; int removeCalls; unsigned long removed;
    CMode* GetCurrentMode() { return &current; }
    void RemoveMode(unsigned long m) { ++removeCalls; removed = m; }
};
struct CThing {
    CTCLook* look; CTCTalk* talk; CTCCreatureModeManager* modes;
    template <class T> bool GetInterface(T** out, long type);
};
template <> bool CThing::GetInterface(CTCLook** out, long type) { if (type != 0x43 || !look) return false; *out = look; return true; }
template <> bool CThing::GetInterface(CTCTalk** out, long type) { if (type != 0x0f || !talk) return false; *out = talk; return true; }
template <> bool CThing::GetInterface(CTCCreatureModeManager** out, long type) { if (type != 0x31 || !modes) return false; *out = modes; return true; }

struct CUserProfileManager {
    bool ProfileLoaded, DefaultSubtitles, ProfileSubtitles;
    bool AreSubtitlesOn() const { return ProfileLoaded ? ProfileSubtitles : DefaultSubtitles; }
};
static CUserProfileManager g_profiles;
struct CPlayerGui { int clearCalls; bool clearArg; bool ShowSubtitleBackground; };
static CPlayerGui g_gui;
static long g_FramesPerSecond = 30;

struct CActionTalkBase {
    CThing* parent; CThing* target;
    long PreviousConversationAttitude; bool AddedConversationMode;
    CThing* GetThing() const { return target; }
    void OnCleanup()
    {
        if (GetThing()) {
            CThing* p = parent;
            if (GetThing() != p) {
                CTCLook* look;
                if (GetThing()->GetInterface(&look, 0x43)) look->StopLooking(2);
            }
        }
        if (!AddedConversationMode) {
            CTCTalk* talk;
            if (parent->GetInterface(&talk, 0x0f)) talk->ConversationAttitude = PreviousConversationAttitude;
        }
        CTCCreatureModeManager* modes;
        if (parent->GetInterface(&modes, 0x31)) {
            if (modes->GetCurrentMode()->GetType() == 27)
                modes->GetCurrentMode()->LingerFrames = (long)(g_FramesPerSecond * 3.0f);
            else
                modes->RemoveMode(27);
        }
        g_gui.ShowSubtitleBackground = true;
        if (!g_profiles.AreSubtitlesOn()) { ++g_gui.clearCalls; g_gui.clearArg = false; }
    }
};

static int Fail(const char* why) { std::printf("00714e00_TEST FAIL (%s)\n", why); return 1; }

int main()
{
    CTCLook look = { 0, 0 }; CTCTalk talk = { 5 };
    CTCCreatureModeManager modes; modes.current.type = 27; modes.current.LingerFrames = 0; modes.removeCalls = 0; modes.removed = 0;
    CThing talker = { 0, &talk, &modes };
    CThing listener = { &look, 0, 0 };
    CActionTalkBase a; a.parent = &talker; a.target = &listener; a.PreviousConversationAttitude = 2; a.AddedConversationMode = false;
    g_profiles.ProfileLoaded = true; g_profiles.ProfileSubtitles = false; g_profiles.DefaultSubtitles = true;
    g_gui.clearCalls = 0; g_gui.ShowSubtitleBackground = false;
    a.OnCleanup();
    if (look.stopCalls != 1 || look.lastTime != 2) return Fail("stop looking");
    if (talk.ConversationAttitude != 2) return Fail("attitude restored");
    if (modes.current.LingerFrames != 90 || modes.removeCalls != 0) return Fail("conversation mode lingers");
    if (!g_gui.ShowSubtitleBackground || g_gui.clearCalls != 1 || g_gui.clearArg) return Fail("subtitles cleared");
    // added mode: attitude kept; another current mode: remove; subtitles on: not cleared; target == talker: no look
    talk.ConversationAttitude = 7; a.AddedConversationMode = true; modes.current.type = 3; a.target = &talker;
    talker.look = &look; g_profiles.ProfileSubtitles = true;
    a.OnCleanup();
    if (look.stopCalls != 1) return Fail("talker is the target");
    if (talk.ConversationAttitude != 7) return Fail("attitude kept");
    if (modes.removeCalls != 1 || modes.removed != 27) return Fail("mode removed");
    if (g_gui.clearCalls != 1) return Fail("subtitles kept");
    // no profile loaded: the default flag decides; no target at all
    g_profiles.ProfileLoaded = false; g_profiles.DefaultSubtitles = false; a.target = 0;
    a.OnCleanup();
    if (g_gui.clearCalls != 2 || look.stopCalls != 1) return Fail("default subtitles");
    std::printf("00714e00_TEST PASS\n");
    return 0;
}