#include <cstdio>
#include <cstdlib>
#include <cstring>
struct C3DVector { float X, Y, Z; };
static int g_compute = 0, g_draw = 0; static bool g_ok = false; static void* g_params = 0; static void* g_camera = 0; static void* g_view = 0; static C3DVector* g_computeA = 0; static C3DVector* g_drawA = 0;
struct CEngineScreenEffectColourFilter { unsigned char _pad[8]; bool m_enabled;
    bool ComputeCorners(void* params, void* camera, C3DVector& a, C3DVector&, C3DVector&, C3DVector&) { ++g_compute; g_params = params; g_camera = camera; g_computeA = &a; return g_ok; }
    void DrawCorners(void* view, C3DVector& a, C3DVector&, C3DVector&, C3DVector&) { ++g_draw; g_view = view; g_drawA = &a; }
    void Render(void* params, void* view, void* camera) { if (m_enabled) { C3DVector a, b, c, d; if (ComputeCorners(params, camera, a, b, c, d)) DrawCorners(view, a, b, c, d); } } };
int main() {
    CEngineScreenEffectColourFilter f; int p = 0, v = 0, c = 0; f.m_enabled = false; f.Render(&p, &v, &c); if (g_compute || g_draw) { std::printf("00b21850_TEST FAIL\n"); return 1; }
    f.m_enabled = true; g_ok = false; f.Render(&p, &v, &c); if (g_compute != 1 || g_draw != 0 || g_camera != &c || g_params != &p) { std::printf("00b21850_TEST FAIL\n"); return 1; }
    g_ok = true; f.Render(&p, &v, &c); if (g_compute != 2 || g_draw != 1 || g_view != &v || g_drawA != g_computeA) { std::printf("00b21850_TEST FAIL\n"); return 1; }
    std::printf("00b21850_TEST PASS\n"); return 0;
}
