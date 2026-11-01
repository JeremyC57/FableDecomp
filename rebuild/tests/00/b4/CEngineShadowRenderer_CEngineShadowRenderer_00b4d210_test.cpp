// Behaviour test for CEngineShadowRenderer::~CEngineShadowRenderer (0x00B4D210): release the default-pool resources first, then
// the members in reverse order (lookup texture, colour texture, colour surface, depth surface, render surface, shadow texture,
// render target's two surfaces), then close the capture block if it is open (restoring the captured state), then the base.
#include <cstdio>
#include <cstring>
static char g_log[64];
static int g_restores;
static void Log(const char* s) { std::strcat(g_log, s); }
struct TexModel { const char* Tag; ~TexModel() { Log(Tag); } };
struct CaptureBlockModel { bool Open; ~CaptureBlockModel() { if (Open) { Open = false; ++g_restores; Log("!"); } } };
struct ComponentModel { virtual ~ComponentModel() { Log("C"); } };
struct ShadowRendererModel : ComponentModel {
    CaptureBlockModel ShadowBufferRenderStateCaptureBlock;
    TexModel RenderToSurface, ZBufferSurface;           // RenderTarget
    TexModel ShadowBufferTexture, RenderSurface, ShadowBufferDepthSurface, ShadowBufferColourSurface;
    TexModel ShadowBufferColourTexture, DepthPackingLookupTexture;
    void OnReleaseDefaultPoolResources() { Log("O"); }
    ~ShadowRendererModel() { OnReleaseDefaultPoolResources(); }
};
static void Fill(ShadowRendererModel& s, bool open)
{
    s.ShadowBufferRenderStateCaptureBlock.Open = open;
    s.RenderToSurface.Tag = "a"; s.ZBufferSurface.Tag = "b"; s.ShadowBufferTexture.Tag = "c"; s.RenderSurface.Tag = "d";
    s.ShadowBufferDepthSurface.Tag = "e"; s.ShadowBufferColourSurface.Tag = "f"; s.ShadowBufferColourTexture.Tag = "g";
    s.DepthPackingLookupTexture.Tag = "h";
}
int main()
{
    ComponentModel* p = new ShadowRendererModel;
    Fill(*(ShadowRendererModel*)p, true);
    delete p;
    if (std::strcmp(g_log, "Ohgfedcba!C") != 0 || g_restores != 1) { std::printf("00b4d210_TEST FAIL\n"); return 1; }
    g_log[0] = 0;
    { ShadowRendererModel s; Fill(s, false); }
    if (std::strcmp(g_log, "OhgfedcbaC") != 0 || g_restores != 1) { std::printf("00b4d210_TEST FAIL\n"); return 1; }
    std::printf("00b4d210_TEST PASS\n");
    return 0;
}