// Behaviour test for CEnginePrimitiveRenderer::Render @ 0x00B33010 -- layer dispatch inside a capture block
#include <cstdio>
static int g_log[64];
static int g_count = 0;
static void Log(int v) { if (g_count < 64) g_log[g_count++] = v; }
enum { RENDER = 100, CLEAR = 200, OCCL_LAYER = 300, SCENE_OCC = 400, SCENE_LIT = 500, SCENE_PLAIN = 600, OPEN = 700, RESTORE = 800,
       REFL_BEGIN = 900, REFL_END = 901, SH_BEGIN = 910, SH_END = 911, CAST_BEGIN = 920, CAST_END = 921, PRE = 930, POST = 931 };
struct SubRenderer {
    long Sizes[16];
    void RenderList(long list) { Log(RENDER + list); }
    void ClearList(long list) { Log(CLEAR + list); Sizes[list] = 0; }
    long GetListSize(long list) const { return Sizes[list]; }
};
struct Scene {
    void RenderOcclusion(long) { Log(SCENE_OCC); }
    void RenderLit(long) { Log(SCENE_LIT); }
    void Render(long) { Log(SCENE_PLAIN); }
};
struct Params { bool CameraOn; unsigned long Layer; unsigned long Flags; };
struct Renderer {
    Scene Scenes[2];
    int SceneCount;
    bool ShadowsActive;
    bool WaterEnabled;
    bool OcclusionEnabled;
    bool ShadowsBegin;
    SubRenderer Sub;
    void Render(const Params& params)
    {
        if (!params.CameraOn)
            return;
        Log(OPEN);
        Log(PRE);
        switch (params.Layer) {
        case 0x20: Log(OCCL_LAYER); break;
        case 2:
            if (OcclusionEnabled)
                for (int i = 0; i < SceneCount; ++i) Scenes[i].RenderOcclusion(params.Layer);
            break;
        case 0x80:
            if (WaterEnabled) {
                Sub.RenderList(0xe);
                if (Sub.GetListSize(0xd) > 0) { Log(REFL_BEGIN); Sub.RenderList(0xd); Log(REFL_END); }
            } else { Sub.ClearList(0xe); Sub.ClearList(0xd); }
            break;
        case 0x100:
            if (WaterEnabled) { Sub.RenderList(8); Sub.RenderList(0xa); } else { Sub.ClearList(8); Sub.ClearList(0xa); }
            break;
        case 0x200:
            if (WaterEnabled) Sub.RenderList(9); else Sub.ClearList(9);
            break;
        case 0x400: Sub.RenderList(0xb); break;
        case 0x1000: Sub.RenderList(7); break;
        case 0x40000: Sub.RenderList(0xc); break;
        case 0x8000000:
            if (Sub.GetListSize(5) > 0) {
                if (ShadowsBegin) { Log(SH_BEGIN); Sub.RenderList(5); Log(SH_END); }
                Sub.ClearList(5);
            }
            break;
        case 0x10000000:
            if (Sub.GetListSize(6) > 0) { Log(CAST_BEGIN); Sub.RenderList(6); Log(CAST_END); Sub.ClearList(6); }
            break;
        default:
            for (int i = 0; i < SceneCount; ++i) {
                if ((params.Flags & 1) && ShadowsActive) Scenes[i].RenderLit(params.Layer);
                else Scenes[i].Render(params.Layer);
            }
            break;
        }
        Sub.RenderList(0);
        if (params.Flags & 2)
            Sub.RenderList(1);
        Log(POST);
        Log(RESTORE);
    }
};
static bool Expect(const int* want, int n)
{
    if (g_count != n) return false;
    for (int i = 0; i < n; ++i) if (g_log[i] != want[i]) return false;
    return true;
}
int main()
{
    bool ok = true;
    Renderer r;
    r.SceneCount = 2; r.ShadowsActive = true; r.WaterEnabled = true; r.OcclusionEnabled = true; r.ShadowsBegin = false;
    for (int i = 0; i < 16; ++i) r.Sub.Sizes[i] = 1;
    Params p;
    p.CameraOn = false; p.Layer = 4; p.Flags = 0;
    g_count = 0; r.Render(p); ok = ok && g_count == 0;
    p.CameraOn = true; p.Layer = 4; p.Flags = 3;
    g_count = 0; r.Render(p);
    { int w[] = { OPEN, PRE, SCENE_LIT, SCENE_LIT, RENDER + 0, RENDER + 1, POST, RESTORE }; ok = ok && Expect(w, 8); }
    p.Layer = 0x80; p.Flags = 0;
    g_count = 0; r.Render(p);
    { int w[] = { OPEN, PRE, RENDER + 0xe, REFL_BEGIN, RENDER + 0xd, REFL_END, RENDER + 0, POST, RESTORE }; ok = ok && Expect(w, 9); }
    r.WaterEnabled = false; p.Layer = 0x100;
    g_count = 0; r.Render(p);
    { int w[] = { OPEN, PRE, CLEAR + 8, CLEAR + 0xa, RENDER + 0, POST, RESTORE }; ok = ok && Expect(w, 7); }
    p.Layer = 0x8000000;
    g_count = 0; r.Render(p);
    { int w[] = { OPEN, PRE, CLEAR + 5, RENDER + 0, POST, RESTORE }; ok = ok && Expect(w, 6); }
    g_count = 0; r.Render(p);   // list 5 now empty: nothing for the layer
    { int w[] = { OPEN, PRE, RENDER + 0, POST, RESTORE }; ok = ok && Expect(w, 5); }
    p.Layer = 0x10000000;
    g_count = 0; r.Render(p);
    { int w[] = { OPEN, PRE, CAST_BEGIN, RENDER + 6, CAST_END, CLEAR + 6, RENDER + 0, POST, RESTORE }; ok = ok && Expect(w, 9); }
    p.Layer = 2;
    g_count = 0; r.Render(p);
    { int w[] = { OPEN, PRE, SCENE_OCC, SCENE_OCC, RENDER + 0, POST, RESTORE }; ok = ok && Expect(w, 7); }
    p.Layer = 0x20;
    g_count = 0; r.Render(p);
    { int w[] = { OPEN, PRE, OCCL_LAYER, RENDER + 0, POST, RESTORE }; ok = ok && Expect(w, 6); }
    if (ok) std::printf("00b33010_TEST PASS\n"); else std::printf("00b33010_TEST FAIL\n");
    return ok ? 0 : 1;
}
