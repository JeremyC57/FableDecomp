// Behaviour test for CEngineScreenEffectColourFilter::~CEngineScreenEffectColourFilter (0x00B5A0D0): the pixel shader then the
// vertex shader drop one reference to their shared data (deleting it on the last one), then the render target's Z-buffer and
// render-to surfaces are uninitialised, then the component base runs.
#include <cstdio>
#include <cstring>
static char g_log[32];
static void Log(const char* s) { std::strcat(g_log, s); }
struct ShaderDataModel { unsigned long ReferenceCount; const char* Tag; virtual ~ShaderDataModel() { Log(Tag); } };
struct ShaderModel {
    ShaderDataModel* Data;
    ~ShaderModel() { ShaderDataModel* data = Data; if (data) { if (--data->ReferenceCount == 0) delete data; } }
};
struct SurfaceModel { const char* Tag; ~SurfaceModel() { Log(Tag); } };
struct RenderTargetModel { SurfaceModel RenderToSurface; SurfaceModel ZBufferSurface; };
struct ComponentModel { virtual ~ComponentModel() { Log("C"); } };
struct ColourFilterModel : ComponentModel {
    RenderTargetModel OldRenderTarget;
    ShaderModel ScreenCopyVertexShader;
    ShaderModel ColourFilterPixelShader;
};
int main()
{
    ShaderDataModel* vs = new ShaderDataModel; vs->ReferenceCount = 1; vs->Tag = "V";
    ShaderDataModel* ps = new ShaderDataModel; ps->ReferenceCount = 2; ps->Tag = "P";
    ComponentModel* f = new ColourFilterModel;
    ColourFilterModel* cf = (ColourFilterModel*)f;
    cf->ScreenCopyVertexShader.Data = vs; cf->ColourFilterPixelShader.Data = ps;
    cf->OldRenderTarget.RenderToSurface.Tag = "R"; cf->OldRenderTarget.ZBufferSurface.Tag = "Z";
    delete f;
    if (std::strcmp(g_log, "VZRC") != 0 || ps->ReferenceCount != 1) { std::printf("00b5a0d0_TEST FAIL\n"); return 1; }
    g_log[0] = 0;
    {
        ColourFilterModel c2;
        c2.ScreenCopyVertexShader.Data = 0; c2.ColourFilterPixelShader.Data = ps;
        c2.OldRenderTarget.RenderToSurface.Tag = "r"; c2.OldRenderTarget.ZBufferSurface.Tag = "z";
    }
    if (std::strcmp(g_log, "PzrC") != 0) { std::printf("00b5a0d0_TEST FAIL\n"); return 1; }
    std::printf("00b5a0d0_TEST PASS\n");
    return 0;
}