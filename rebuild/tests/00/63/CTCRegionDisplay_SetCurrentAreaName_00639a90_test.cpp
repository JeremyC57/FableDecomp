// Behaviour test for CTCRegionDisplay::SetCurrentAreaName (0x00639A90): a non-empty name is copied and stamps the frame, an empty
// one clears both strings and the alpha; -1 selects 5 s, showNow forces both alphas to 255, TimeOut = time != 0.
#include <cstdio>
#include <stddef.h>
static long g_frame = 0; static int g_getWorldCalls = 0;
class CWorld { public: static long GetWorldFrame() { return g_frame; } };
static bool SameText(const wchar_t* a, const wchar_t* b)
{
    if (!a) a = L""; if (!b) b = L"";
    while (*a && *a == *b) { ++a; ++b; }
    return *a == *b;
}
class CWideString { public:
    const wchar_t* m_pData;
    bool operator!=(const wchar_t* text) const { return !SameText(m_pData, text); }
    CWideString& operator=(const CWideString& other) { m_pData = other.m_pData; return *this; }
    const CWideString& operator=(const wchar_t* text) { m_pData = text; return *this; }
};
static CWorld g_world;
class CTCBase { public:
    virtual void v00() { }
    void* ParentThing;
    CWorld* GetWorld() { ++g_getWorldCalls; return &g_world; }
};
class CTCRegionDisplay : public CTCBase { public:
    CWideString   CurrentAreaName;
    CWideString   CurrentAreaNameExtra;
    long          FrameNameChanged;
    long          CurrentAlpha;
    long          LastAlpha;
    float         DisplayTime;
    bool          TimeOut;
    bool          InfoDisplay;
    bool          UpdateWhilePaused;
    void SetCurrentAreaName(const CWideString& name, float displayTime, bool infoDisplay, bool showNow, bool updateWhilePaused);
};

void CTCRegionDisplay::SetCurrentAreaName(const CWideString& name, float displayTime, bool infoDisplay, bool showNow, bool updateWhilePaused)
{
    if (name != L"") {
        CurrentAreaName = name;
        CurrentAreaNameExtra = L"";
        FrameNameChanged = GetWorld()->GetWorldFrame();
        LastAlpha = 0;
    } else {
        CurrentAreaName = L"";
        CurrentAreaNameExtra = L"";
        CurrentAlpha = 0;
    }
    if (displayTime == -1.0f)
        displayTime = 5.0f;
    if (showNow)
        CurrentAlpha = LastAlpha = 255;
    DisplayTime = displayTime;
    TimeOut = displayTime != 0.0f;
    InfoDisplay = infoDisplay;
    UpdateWhilePaused = updateWhilePaused;
}

int main()
{
    int bad = 0;
    CTCRegionDisplay d;
    d.CurrentAreaName.m_pData = L"Old"; d.CurrentAreaNameExtra.m_pData = L"Extra";
    d.FrameNameChanged = 7; d.CurrentAlpha = 40; d.LastAlpha = 30; d.DisplayTime = 0; d.TimeOut = d.InfoDisplay = d.UpdateWhilePaused = false;
    CWideString name; name.m_pData = L"Bowerstone";
    g_frame = 1234;
    d.SetCurrentAreaName(name, -1.0f, true, false, true);
    if (d.CurrentAreaName.m_pData != name.m_pData || !SameText(d.CurrentAreaNameExtra.m_pData, L"")) bad |= 1;
    if (d.FrameNameChanged != 1234 || d.LastAlpha != 0 || d.CurrentAlpha != 40 || g_getWorldCalls != 1) bad |= 2;
    if (d.DisplayTime != 5.0f || !d.TimeOut || !d.InfoDisplay || !d.UpdateWhilePaused) bad |= 4;
    CWideString empty; empty.m_pData = L"";
    d.SetCurrentAreaName(empty, 0.0f, false, true, false);
    if (!SameText(d.CurrentAreaName.m_pData, L"") || d.FrameNameChanged != 1234 || g_getWorldCalls != 1) bad |= 8;
    if (d.CurrentAlpha != 255 || d.LastAlpha != 255 || d.DisplayTime != 0.0f || d.TimeOut || d.InfoDisplay || d.UpdateWhilePaused) bad |= 16;
    d.SetCurrentAreaName(empty, 3.0f, false, false, false);
    if (d.CurrentAlpha != 0 || d.LastAlpha != 255 || d.DisplayTime != 3.0f || !d.TimeOut) bad |= 32;
    if (bad) { std::printf("00639a90_TEST FAIL\n"); return 1; }
    std::printf("00639a90_TEST PASS\n");
    return 0;
}