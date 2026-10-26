// Behaviour test for 0x009E9EF0 NProgressDisplay::StartProgress: forwards the six flags only when a display is installed.
#include <cstdio>

class CAProgressDisplay
{
public:
    virtual ~CAProgressDisplay() {}
    virtual void StartProgress(bool a, bool b, bool c, bool d, bool e, bool f) = 0;
};
class CTestDisplay : public CAProgressDisplay
{
public:
    int Calls; int Mask;
    CTestDisplay() : Calls(0), Mask(0) {}
    virtual void StartProgress(bool a, bool b, bool c, bool d, bool e, bool f)
    {
        ++Calls;
        Mask = (a ? 1 : 0) | (b ? 2 : 0) | (c ? 4 : 0) | (d ? 8 : 0) | (e ? 16 : 0) | (f ? 32 : 0);
    }
};
template <class T> class CCountedPointer { public: T* Data; void* Info; T* operator->() const { return Data; } };
static CCountedPointer<CAProgressDisplay> g_ProgressDisplay = { 0, 0 };

static void StartProgress(bool a, bool b, bool c, bool d, bool e, bool f)
{
    if (g_ProgressDisplay.Data)
        g_ProgressDisplay->StartProgress(a, b, c, d, e, f);
}

int main()
{
    StartProgress(true, true, true, true, true, true);            // no display: nothing happens
    CTestDisplay display;
    g_ProgressDisplay.Data = &display;
    StartProgress(true, false, true, false, false, true);
    bool ok = display.Calls == 1 && display.Mask == (1 | 4 | 32);
    StartProgress(false, true, false, true, true, false);
    ok = ok && display.Calls == 2 && display.Mask == (2 | 8 | 16);
    if (!ok) { std::printf("009e9ef0_TEST FAIL\n"); return 1; }
    std::printf("009e9ef0_TEST PASS\n");
    return 0;
}