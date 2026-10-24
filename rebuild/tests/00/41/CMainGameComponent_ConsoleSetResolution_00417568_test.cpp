#include <cstdio>
#include <cstring>
#include <cstdarg>
// Behaviour test for CMainGameComponent::ConsoleSetResolution (retail 0x00417568): the three integer console
// arguments are formatted into a (discarded) message and then drive PreChangeResolution, SetDisplayMode and
// PostChangeResolution with the requested width, height and depth.
static char g_message[128];
static int g_messageDestroyed = 0;
struct CCharString {
    CCharString() { }
    ~CCharString() { ++g_messageDestroyed; }
    void Format(const char* format, ...) { va_list a; va_start(a, format); std::vsprintf(g_message, format, a); va_end(a); }
};
struct CGenericVar { long Type; long ValSLONG; };
struct CArgs { CGenericVar* _M_start; const CGenericVar& operator[](unsigned int n) const { return _M_start[n]; } };
struct C2DExtentsI { long Width, Height; C2DExtentsI(long w, long h) : Width(w), Height(h) {} };
static int g_order = 0, g_preAt = 0, g_setAt = 0, g_postAt = 0;
static long g_reqW = 0, g_reqH = 0; static unsigned long g_reqDepth = 0, g_reqAA = 9, g_reqRefresh = 9;
static unsigned long g_postW = 0, g_postH = 0, g_postD = 0;
struct CDisplayManager {
    int SetDisplayMode(const C2DExtentsI& r, unsigned long bpp, int aa, unsigned long refresh)
    { g_setAt = ++g_order; g_reqW = r.Width; g_reqH = r.Height; g_reqDepth = bpp; g_reqAA = aa; g_reqRefresh = refresh; return 0; }
};
static CDisplayManager g_manager;
static CDisplayManager* g_pDisplayManager = &g_manager;
struct CDisplayEngine { void PreChangeResolution() { g_preAt = ++g_order; } };
struct CMainGameComponent {
    CDisplayEngine* PDisplayEngine;
    void PostChangeResolution(unsigned long w, unsigned long h, unsigned long d) { g_postAt = ++g_order; g_postW = w; g_postH = h; g_postD = d; }
    static void ConsoleSetResolution(CMainGameComponent& component, const CArgs& args)
    {
        CCharString message;
        unsigned long width = args[0].ValSLONG;
        unsigned long height = args[1].ValSLONG;
        unsigned long depth = args[2].ValSLONG;
        message.Format("Nearest supported resolution is (%d,%d,%d)", (int)width, (int)height, (int)depth);
        component.PDisplayEngine->PreChangeResolution();
        g_pDisplayManager->SetDisplayMode(C2DExtentsI(width, height), depth, 0, 0);
        component.PostChangeResolution(width, height, depth);
    }
};
int main()
{
    CGenericVar v[3]; v[0].ValSLONG = 800; v[1].ValSLONG = 600; v[2].ValSLONG = 32;
    CArgs args; args._M_start = v;
    CDisplayEngine engine; CMainGameComponent c; c.PDisplayEngine = &engine;
    CMainGameComponent::ConsoleSetResolution(c, args);
    int ok = std::strcmp(g_message, "Nearest supported resolution is (800,600,32)") == 0 && g_messageDestroyed == 1
          && g_preAt == 1 && g_setAt == 2 && g_postAt == 3 && g_reqW == 800 && g_reqH == 600 && g_reqDepth == 32
          && g_reqAA == 0 && g_reqRefresh == 0 && g_postW == 800 && g_postH == 600 && g_postD == 32;
    if (!ok) { std::printf("00417568_TEST FAIL\n"); return 1; }
    std::printf("00417568_TEST PASS\n");
    return 0;
}