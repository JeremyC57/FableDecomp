#include <cstdio>
// Behaviour test for 00492f69: CSubtitleRenderer members used by the body (retail layout names from the PDB), with
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
static int g_wideDtors = 0;
class CWideString {
public:
    const char* m_tag;
    CWideString(const char* tag) : m_tag(tag) {}
    ~CWideString() { ++g_wideDtors; }
};
struct SSetText { CFontBank* font; const CWideString* text; const CWideString* name; const CWideString* b1; const CWideString* b2;
                  const char* b3Tag; unsigned long flags; float time; bool a; bool b; int dtorsAtCall; };
static SSetText g_set;
class CSubtitleRenderer {
public:
    unsigned long      DisplayFlags;
    bool               InCutscene;
    bool               CutsceneSkipped;
    bool               CutsceneSkippable;
    bool               CutsceneActionMode;
    bool               DisplayingText;
    CFadeInFadeOutBase SkipCutsceneTimer;
    void SetText(CFontBank* font, const CWideString& text, const CWideString& nameText, const CWideString& button1Text,
                 const CWideString& button2Text, const CWideString& button3Text, unsigned long displayFlags,
                 float displayTime, bool updateWhilePaused, bool changePlayerMode)
    {
        g_set.font = font; g_set.text = &text; g_set.name = &nameText; g_set.b1 = &button1Text; g_set.b2 = &button2Text;
        g_set.b3Tag = button3Text.m_tag; g_set.flags = displayFlags; g_set.time = displayTime; g_set.a = updateWhilePaused;
        g_set.b = changePlayerMode; g_set.dtorsAtCall = g_wideDtors;
    }
    void SetText(CFontBank* font, const CWideString& text, const CWideString& nameText, const CWideString& button1Text,
                 const CWideString& button2Text, unsigned long displayFlags, float displayTime,
                 bool updateWhilePaused, bool changePlayerMode);
};
void CSubtitleRenderer::SetText(CFontBank* font, const CWideString& text, const CWideString& nameText, const CWideString& button1Text,
                                const CWideString& button2Text, unsigned long displayFlags, float displayTime,
                                bool updateWhilePaused, bool changePlayerMode)
{
    CWideString button3Text("");
    SetText(font, text, nameText, button1Text, button2Text, button3Text, displayFlags, displayTime, updateWhilePaused, changePlayerMode);
}
#define FAIL do { std::printf("00492f69_TEST FAIL\n"); return 1; } while (0)
int main()
{
    CSubtitleRenderer r; CFontBank font;
    CWideString t("t"), n("n"), b1("b1"), b2("b2");
    r.SetText(&font, t, n, b1, b2, 0x41, 3.5f, true, false);
    if (g_set.font != &font || g_set.text != &t || g_set.name != &n || g_set.b1 != &b1 || g_set.b2 != &b2 || g_set.b3Tag[0] != 0
        || g_set.flags != 0x41 || g_set.time != 3.5f || !g_set.a || g_set.b || g_set.dtorsAtCall != 0 || g_wideDtors != 1) FAIL;
    std::printf("00492f69_TEST PASS\n");
    return 0;
}