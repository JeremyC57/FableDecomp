#include <cstdio>
// Behaviour test for 004908da: CSubtitleRenderer members used by the body (retail layout names from the PDB), with
// recording stand-ins for the out-of-line callees.
struct C2DVector { float X, Y; };
struct CRGBColour { unsigned char B, G, R, A; };
class CFontBank { public: int m_id; };
struct CFadeInFadeOutBase {
    bool  Active;
    float FadeInTime;
    float FadeOutTime;
    bool  Closing;
    bool  Opening;
    float OpenTimer;
    float CloseTimer;
};
#include <cstring>
static int g_stringDtors = 0;
class CCharString {
public:
    const char* m_text;
    CCharString(const char* text, long length = -1) : m_text(text) { (void)length; }
    ~CCharString() { ++g_stringDtors; }
};
static const char* g_overrideText = 0;
static int g_overrideCalls = 0;
static bool g_modeAtOverride = false;
class CSubtitleRenderer {
public:
    unsigned long      DisplayFlags;
    bool               InCutscene;
    bool               CutsceneSkipped;
    bool               CutsceneSkippable;
    bool               CutsceneActionMode;
    bool               DisplayingText;
    CFadeInFadeOutBase SkipCutsceneTimer;
    void SetButton1OverrideText(const CCharString& text) { g_overrideText = text.m_text; ++g_overrideCalls; g_modeAtOverride = CutsceneActionMode; }
    void SetCutsceneActionMode(bool actionMode, const CCharString& buttonText);
};
void CSubtitleRenderer::SetCutsceneActionMode(bool actionMode, const CCharString& buttonText)
{
    if (actionMode) {
        SetButton1OverrideText(buttonText);
        CutsceneActionMode = actionMode;
    } else {
        CutsceneActionMode = false;
        SetButton1OverrideText("");
    }
}
#define FAIL do { std::printf("004908da_TEST FAIL\n"); return 1; } while (0)
int main()
{
    CSubtitleRenderer r; r.CutsceneActionMode = false;
    CCharString text("PRESS A");
    r.SetCutsceneActionMode(true, text);
    if (!r.CutsceneActionMode || std::strcmp(g_overrideText, "PRESS A") != 0 || g_modeAtOverride || g_stringDtors != 0) FAIL;
    r.SetCutsceneActionMode(false, text);
    if (r.CutsceneActionMode || g_overrideText[0] != 0 || g_modeAtOverride || g_overrideCalls != 2 || g_stringDtors != 1) FAIL;
    std::printf("004908da_TEST PASS\n");
    return 0;
}