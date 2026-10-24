// Behaviour test for 0x009E9F40 NProgressDisplay::DisplayProgress: forwards text/progress/flags only when installed.
#include <cstdio>

struct CCharString { const char* Text; };

class CAProgressDisplay
{
public:
    virtual ~CAProgressDisplay() {}
    virtual void DisplayProgress(const CCharString& text, float progress, bool a, bool b) = 0;
};
class CTestDisplay : public CAProgressDisplay
{
public:
    int Calls; const CCharString* Last; float Progress; bool A, B;
    CTestDisplay() : Calls(0), Last(0), Progress(0.0f), A(false), B(false) {}
    virtual void DisplayProgress(const CCharString& text, float progress, bool a, bool b)
    {
        ++Calls; Last = &text; Progress = progress; A = a; B = b;
    }
};
template <class T> struct CCountedPointer { T* Data; void* Info; T* operator->() const { return Data; } };
static CCountedPointer<CAProgressDisplay> g_ProgressDisplay = { 0, 0 };

static void DisplayProgress(const CCharString& text, float progress, bool a, bool b)
{
    if (g_ProgressDisplay.Data)
        g_ProgressDisplay->DisplayProgress(text, progress, a, b);
}

int main()
{
    CCharString loading = { "Loading" };
    DisplayProgress(loading, 0.5f, true, true);
    CTestDisplay display;
    g_ProgressDisplay.Data = &display;
    DisplayProgress(loading, 0.25f, true, false);
    float d = display.Progress - 0.25f;
    bool ok = display.Calls == 1 && display.Last == &loading && d < 0.0001f && d > -0.0001f && display.A && !display.B;
    if (!ok) { std::printf("009e9f40_TEST FAIL\n"); return 1; }
    std::printf("009e9f40_TEST PASS\n");
    return 0;
}