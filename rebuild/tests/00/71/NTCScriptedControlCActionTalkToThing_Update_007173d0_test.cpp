// Behaviour test for 0x007173D0 NTCScriptedControl::CActionTalkToThing::Update
#include <cstdio>
class CProfile { public:
    bool UseNewProfileValues, SubtitlingEnabled, SubtitlingEnabledNewProfile;
    bool IsSubtitlingEnabled() const { if (UseNewProfileValues) return SubtitlingEnabledNewProfile; return SubtitlingEnabled; }
};
static CProfile g_profile;
static bool g_displayingText;
static int g_messageType;                          // the pending message event type, 0 = none
static void* FindMostRecentMessage(long type) { return g_messageType == type ? (void*)&g_messageType : 0; }
struct CTalk {
    long UpdateCounter, GameTurnDuration; bool SkippedCurrentLine; int baseUpdates;
    void SkipCurrentLine() { UpdateCounter = GameTurnDuration; SkippedCurrentLine = true; }
    void BaseUpdate() { ++baseUpdates; }
    void Update()
    {
        if (g_profile.IsSubtitlingEnabled()) {
            if (!g_displayingText)
                SkipCurrentLine();
        } else {
            if (FindMostRecentMessage(0x11))
                SkipCurrentLine();
        }
        BaseUpdate();
    }
};
int main()
{
    CTalk t; t.UpdateCounter = 3; t.GameTurnDuration = 50; t.SkippedCurrentLine = false; t.baseUpdates = 0;
    g_profile.UseNewProfileValues = false; g_profile.SubtitlingEnabled = true; g_profile.SubtitlingEnabledNewProfile = false;
    g_displayingText = true; g_messageType = 0x11;
    t.Update();
    int ok = t.UpdateCounter == 3 && !t.SkippedCurrentLine && t.baseUpdates == 1;
    g_displayingText = false;
    t.Update();
    ok = ok && t.UpdateCounter == 50 && t.SkippedCurrentLine && t.baseUpdates == 2;
    t.UpdateCounter = 4; t.SkippedCurrentLine = false; g_profile.UseNewProfileValues = true; g_messageType = 0;
    t.Update();
    ok = ok && t.UpdateCounter == 4 && !t.SkippedCurrentLine && t.baseUpdates == 3;
    g_messageType = 0x11;
    t.Update();
    ok = ok && t.UpdateCounter == 50 && t.SkippedCurrentLine && t.baseUpdates == 4;
    if (!ok) { std::printf("007173d0_TEST FAIL\n"); return 1; }
    std::printf("007173d0_TEST PASS\n");
    return 0;
}