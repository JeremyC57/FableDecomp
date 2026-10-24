// Behaviour test for CDisplayEngine::Init (0x004350D0): two progress reports in order (graphics, then engine, progress -1, both
// flags false), game drawing on from frame 0, the virtual engine initialisation run once, the refresh clock zeroed, camera inputs
// on and a fresh view manager owned by the counted pointer.
#include <cstdio>
#include <cstring>
class CCharString {
public:
    CCharString(const char* text, long length = -1) { (void)length; std::strcpy(buf, text); }
    char buf[64];
};
static char g_reports[2][64]; static int g_reportCount = 0; static int g_badArgs = 0;
namespace NProgressDisplay {
void DisplayProgress(const CCharString& text, float progress, bool flag1, bool flag2)
{
    if (g_reportCount < 2) std::strcpy(g_reports[g_reportCount], text.buf);
    ++g_reportCount;
    if (progress != -1.0f || flag1 || flag2) g_badArgs = 1;
}
}
class CDisplayViewManager { public: CDisplayViewManager() : m_first(0), m_second(0) {} long m_first, m_second; };
template <class T> class CCountedPointer {
public:
    CCountedPointer() : Data(0), resets(0) {}
    ~CCountedPointer() { delete Data; }
    void Reset(T* data) { delete Data; Data = data; ++resets; }
    T* Data; int resets;
};
class CBaseClass { public: virtual ~CBaseClass() {} };
class CDisplayEngine : public CBaseClass {
public:
    CDisplayEngine() : initEngineCalls(0), Frame(77), DrawGame(false), CameraInputsOn(false), LastRefreshTime(12.5) {}
    virtual bool InitEngine() { ++initEngineCalls; if (g_reportCount != 2) g_badArgs = 1; return true; }
    void Init()
    {
        NProgressDisplay::DisplayProgress("Init Display Engine: Init Graphics", -1.0f, false, false);
        DrawGame = true;
        Frame = 0;
        NProgressDisplay::DisplayProgress("Init Display Engine: Init Engine", -1.0f, false, false);
        InitEngine();
        LastRefreshTime = 0.0;
        CameraInputsOn = true;
        PViewManager.Reset(new CDisplayViewManager);
    }
    int initEngineCalls; long Frame; bool DrawGame; bool CameraInputsOn; double LastRefreshTime;
    CCountedPointer<CDisplayViewManager> PViewManager;
};
int main()
{
    CDisplayEngine e;
    e.Init();
    int bad = g_badArgs;
    if (g_reportCount != 2 || std::strcmp(g_reports[0], "Init Display Engine: Init Graphics") || std::strcmp(g_reports[1], "Init Display Engine: Init Engine")) bad = 1;
    if (e.initEngineCalls != 1 || e.Frame != 0 || !e.DrawGame || !e.CameraInputsOn || e.LastRefreshTime != 0.0) bad = 1;
    if (!e.PViewManager.Data || e.PViewManager.resets != 1 || e.PViewManager.Data->m_first != 0) bad = 1;
    if (bad) { std::printf("004350d0_TEST FAIL\n"); return 1; }
    std::printf("004350d0_TEST PASS\n");
    return 0;
}