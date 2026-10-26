#include <cstdio>
// Behaviour test for 0048dbc3: CSubtitleRenderer members used by the body (retail layout names from the PDB), with
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
static float g_lastY = 0.0f, g_lastX = 0.0f;
static float GFAYToVY(float y) { g_lastY = y; return y * 2.0f; }
static float GFAXToVX(float x) { g_lastX = x; return x * 2.0f; }
static double g_screenHeight = 480.0;
static double g_screenWidth = 640.0;
class CSubtitleRenderer {
public:
    unsigned long      DisplayFlags;
    bool               InCutscene;
    bool               CutsceneSkipped;
    bool               CutsceneSkippable;
    bool               CutsceneActionMode;
    bool               DisplayingText;
    CFadeInFadeOutBase SkipCutsceneTimer;
    float GetBlackBarHeight() const;
};
float CSubtitleRenderer::GetBlackBarHeight() const
{
    float height = GFAYToVY((float)g_screenHeight);
    return (height - GFAXToVX((float)g_screenWidth) * 0.5625f) * 0.5f;
}
#define FAIL do { std::printf("0048dbc3_TEST FAIL\n"); return 1; } while (0)
int main()
{
    CSubtitleRenderer r;
    float h = r.GetBlackBarHeight();
    if (g_lastY != 480.0f || g_lastX != 640.0f || h != (960.0f - 1280.0f * 0.5625f) * 0.5f || h != 120.0f) FAIL;
    g_screenHeight = 1024.0; g_screenWidth = 1280.0;
    if (r.GetBlackBarHeight() != (2048.0f - 2560.0f * 0.5625f) * 0.5f) FAIL;
    std::printf("0048dbc3_TEST PASS\n");
    return 0;
}