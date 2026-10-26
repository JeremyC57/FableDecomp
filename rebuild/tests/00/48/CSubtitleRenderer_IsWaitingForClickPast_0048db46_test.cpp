#include <cstdio>
// Behaviour test for 0048db46: CSubtitleRenderer members used by the body (retail layout names from the PDB), with
// recording stand-ins for the out-of-line callees.
class C2DVector { public: float X, Y; };
class CRGBColour { public: unsigned char B, G, R, A; };
class CFontBank { public: int m_id; };
class CFadeInFadeOutBase { public:
    bool  Active;
    float FadeInTime;
    float FadeOutTime;
    bool  Closing;
    bool  Opening;
    float OpenTimer;
    float CloseTimer;
};
class CUserProfileManager {
public:
    bool UseNewProfileValues;
    bool SubtitlingEnabled;
    bool SubtitlingEnabledNewProfile;
    static CUserProfileManager& Get();
    bool IsSubtitlingEnabled() const
    {
        if (UseNewProfileValues)
            return SubtitlingEnabledNewProfile;
        return SubtitlingEnabled;
    }
};
static CUserProfileManager g_profiles;
static int g_profileGets = 0;
CUserProfileManager& CUserProfileManager::Get() { ++g_profileGets; return g_profiles; }
class CSubtitleRenderer {
public:
    unsigned long      DisplayFlags;
    bool               InCutscene;
    bool               CutsceneSkipped;
    bool               CutsceneSkippable;
    bool               CutsceneActionMode;
    bool               DisplayingText;
    CFadeInFadeOutBase SkipCutsceneTimer;
    bool IsWaitingForClickPast() const;
};
bool CSubtitleRenderer::IsWaitingForClickPast() const
{
    if (DisplayingText) {
        if (DisplayFlags & 0x48)
            return true;
        if (DisplayFlags & 2) {
            if (!InCutscene) {
                if (CUserProfileManager::Get().IsSubtitlingEnabled())
                    return true;
            }
        }
    }
    return false;
}
#define FAIL do { std::printf("0048db46_TEST FAIL\n"); return 1; } while (0)
int main()
{
    CSubtitleRenderer r; r.DisplayingText = false; r.DisplayFlags = 0x40; r.InCutscene = false;
    g_profiles.UseNewProfileValues = false; g_profiles.SubtitlingEnabled = true; g_profiles.SubtitlingEnabledNewProfile = false;
    if (r.IsWaitingForClickPast()) FAIL;
    r.DisplayingText = true;
    if (!r.IsWaitingForClickPast() || g_profileGets != 0) FAIL;
    r.DisplayFlags = 0x8; if (!r.IsWaitingForClickPast()) FAIL;
    r.DisplayFlags = 0x1; if (r.IsWaitingForClickPast() || g_profileGets != 0) FAIL;
    r.DisplayFlags = 0x2; if (!r.IsWaitingForClickPast() || g_profileGets != 1) FAIL;
    g_profiles.UseNewProfileValues = true; if (r.IsWaitingForClickPast()) FAIL;
    g_profiles.SubtitlingEnabledNewProfile = true; if (!r.IsWaitingForClickPast()) FAIL;
    r.InCutscene = true; if (r.IsWaitingForClickPast() || g_profileGets != 3) FAIL;
    std::printf("0048db46_TEST PASS\n");
    return 0;
}