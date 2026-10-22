// Behaviour test for CEngineCamera::CEngineCamera(bool) (0x00B31700): base + member defaults, and the
// flag branch's local array of eight 3s that is built and released.
#include <cstdio>
#include <cstdlib>

static int g_pushes = 0, g_frees = 0;

struct CEngineComponent {
    long GlobalPointerAddress; bool m_enabled; bool KeepAlive;
    CEngineComponent() : GlobalPointerAddress(0), m_enabled(true), KeepAlive(false) {}
};
struct CEngineCameraDesc { bool m_valid; CEngineCameraDesc() : m_valid(false) {} };
struct CLongArray {
    long* m_pFirst; long* m_pLast; long* m_pEnd;
    CLongArray() : m_pFirst(0), m_pLast(0), m_pEnd(0) {}
    ~CLongArray() { if (m_pFirst) { ++g_frees; std::free(m_pFirst); } }
    void push_back(const long& value)
    {
        if (m_pLast == m_pEnd) {
            long n = (long)(m_pLast - m_pFirst), cap = n ? n * 2 : 1;
            long* p = (long*)std::malloc(cap * sizeof(long));
            for (long i = 0; i < n; ++i) p[i] = m_pFirst[i];
            std::free(m_pFirst);
            m_pFirst = p; m_pLast = p + n; m_pEnd = p + cap;
        }
        *m_pLast++ = value;
        ++g_pushes;
    }
};
struct CDebugScriptArray { void* a; void* b; void* c; CDebugScriptArray() : a(0), b(0), c(0) {} };
struct CEngineCamera : CEngineComponent {
    void* GameCamera; void* GameWindow;
    CEngineCameraDesc CameraDesc;
    long ConsoleOverrideViewPortWidth, ConsoleOverrideViewPortHeight;
    CDebugScriptArray DebugScript;
    long CurrentDebugScriptElement;
    bool DebugScriptRunning;
    long lastScriptSum;
    CEngineCamera(bool buildDebugScript)
    {
        ConsoleOverrideViewPortWidth = -1;
        ConsoleOverrideViewPortHeight = -1;
        GameCamera = 0;
        GameWindow = 0;
        DebugScriptRunning = false;
        CurrentDebugScriptElement = 0;
        lastScriptSum = 0;
        if (buildDebugScript) {
            CLongArray script;
            for (long i = 0; i < 8; ++i)
                script.push_back(3);
            for (long* p = script.m_pFirst; p != script.m_pLast; ++p) lastScriptSum += *p;
        }
    }
};

int main()
{
    CEngineCamera a(false);
    if (a.ConsoleOverrideViewPortWidth != -1 || a.ConsoleOverrideViewPortHeight != -1 || a.GameCamera || a.GameWindow
        || a.DebugScriptRunning || a.CurrentDebugScriptElement != 0 || a.CameraDesc.m_valid || !a.m_enabled || g_pushes != 0) {
        std::printf("00b31700_TEST FAIL\n"); return 1;
    }
    CEngineCamera b(true);
    if (g_pushes != 8 || g_frees != 1 || b.lastScriptSum != 24 || b.DebugScript.a != 0) {
        std::printf("00b31700_TEST FAIL\n"); return 1;
    }
    std::printf("00b31700_TEST PASS\n");
    return 0;
}