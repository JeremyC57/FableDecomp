// Behaviour test for CUserProfileManager::CaptureBackBufferForUIBackground (00406cc0): call sequence and result.
#include <cstdio>
#include <cstring>
static char g_log[64];
struct Info { long w[13]; };
struct Engine { void Enable3D(bool e) { std::strcat(g_log, e ? "3" : "a"); } void Enable2D(bool e) { std::strcat(g_log, e ? "2" : "b"); } };
struct Display { Engine* Engine3D; const Info* seen; void DoRender(Info* i, bool, bool) { std::strcat(g_log, "R"); seen = i; } };
struct Main { Display* PDisplayEngine; Info LastInterpolationInfo; };
struct Tex { int id; };
static Tex g_tex = { 5 };
static bool g_enabled = true;
static Engine g_engine; static Display g_display = { &g_engine, 0 }; static Main g_main;
struct Mgr {
    Tex* PBackBufferTexture;
    void Capture()
    {
        if (g_enabled) {
            g_main.PDisplayEngine->Engine3D->Enable3D(true);
            g_main.PDisplayEngine->Engine3D->Enable2D(false);
            Info info = g_main.LastInterpolationInfo;
            g_main.PDisplayEngine->DoRender(&info, false, false);
            g_main.PDisplayEngine->DoRender(&info, false, false);
            g_main.PDisplayEngine->Engine3D->Enable3D(false);
            g_main.PDisplayEngine->Engine3D->Enable2D(true);
            PBackBufferTexture = &g_tex;
        } else PBackBufferTexture = 0;
    }
};
int main()
{
    g_main.PDisplayEngine = &g_display; for (int i = 0; i < 13; ++i) g_main.LastInterpolationInfo.w[i] = i;
    Mgr m; m.PBackBufferTexture = 0; m.Capture();
    bool ok = !std::strcmp(g_log, "3bRRa2") && m.PBackBufferTexture == &g_tex && g_display.seen != &g_main.LastInterpolationInfo;
    g_enabled = false; m.Capture(); ok = ok && m.PBackBufferTexture == 0;
    if (!ok) { std::printf("00406cc0_TEST FAIL\n"); return 1; }
    std::printf("00406cc0_TEST PASS\n");
    return 0;
}