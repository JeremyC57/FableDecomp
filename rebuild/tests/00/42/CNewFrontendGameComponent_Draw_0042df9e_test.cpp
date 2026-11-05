// Behaviour test for 0x0042DF9E: CNewFrontendGameComponent::Draw -- the frame's render sequence, with the error message drawn only when present
#include <cstdio>
#include <string>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
static std::string g_log;
static void Log(const char* s) { g_log += s; g_log += ","; }
struct C2DBoxF { float TLX, TLY, BRX, BRY; C2DBoxF(float a, float b, float c, float d) : TLX(a), TLY(b), BRX(c), BRY(d) { } };
struct CRGBColour { unsigned char B, G, R, A; CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; } };
static double VIRTUAL_SCREEN_WIDTH = 65536.0, VIRTUAL_SCREEN_HEIGHT = 65536.0;
struct CRenderManager {
    bool Fog; int MipMode; C2DBoxF Window; CRGBColour Clear;
    CRenderManager() : Fog(true), MipMode(-1), Window(0, 0, 0, 0), Clear(1, 1, 1, 1) { }
    void SetFogEnable(bool e) { Fog = e; Log("fog"); }
    void SetVWindow(const C2DBoxF& b) { Window = b; Log("vwindow"); }
    void ClearRenderTarget(const CRGBColour& c, unsigned long) { Clear = c; Log("clear"); }
    void SetTextureMipMapMode(long, int m) { MipMode = m; Log("mip"); }
    void DrawBufferedPolys() { Log("polys"); }
    void Render2DDrawList(bool) { Log("list"); }
};
struct CDisplayManager {
    void GetVVisibleTelevisionScreenArea() { Log("area"); }
    void BeginRender() { Log("begin"); } void EndRender() { Log("end"); } void SwapScreens() { Log("swap"); }
};
static CRenderManager g_render; static CDisplayManager g_display; static bool g_hasError = false;
struct CEngine { void Render(int, unsigned long) { Log("engine"); } };
struct CNewFrontendGameComponent {
    CEngine Engine; float m_fLastRenderTime;
    void Draw(int interpolation) {
        m_fLastRenderTime = 12.5f;
        C2DBoxF screen(0.0f, 0.0f, (float)VIRTUAL_SCREEN_WIDTH, (float)VIRTUAL_SCREEN_HEIGHT);
        g_render.SetFogEnable(false);
        g_render.SetVWindow(screen);
        g_display.GetVVisibleTelevisionScreenArea();
        g_render.ClearRenderTarget(CRGBColour(0, 0, 0, 255), 0);
        g_display.BeginRender();
        Log("frontend");
        if (g_hasError) Log("error");
        Engine.Render(interpolation, 0);
        g_render.SetTextureMipMapMode(0, 1);
        g_render.DrawBufferedPolys(); g_render.Render2DDrawList(true);
        Log("ime");
        g_render.DrawBufferedPolys(); g_render.Render2DDrawList(true);
        g_display.EndRender(); g_display.SwapScreens();
    }
};
int main()
{
    CNewFrontendGameComponent c; c.Draw(0);
    CHECK(g_log == "fog,vwindow,area,clear,begin,frontend,engine,mip,polys,list,ime,polys,list,end,swap,");
    CHECK(!g_render.Fog && g_render.MipMode == 1 && g_render.Window.BRX == 65536.0f && g_render.Clear.A == 255 && g_render.Clear.R == 0);
    g_log = ""; g_hasError = true; c.Draw(0);
    CHECK(g_log.find("frontend,error,engine") != std::string::npos);
    if (g_fail) { std::printf("0042df9e_TEST FAIL\n"); return 1; }
    std::printf("0042df9e_TEST PASS\n");
    return 0;
}
