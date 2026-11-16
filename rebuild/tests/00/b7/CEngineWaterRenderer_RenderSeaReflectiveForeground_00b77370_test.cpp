// Behaviour test for CEngineWaterRenderer::RenderSeaReflectiveForeground (0x00B77370): a model of the reflective sea foreground pass --
// the capture block is always opened and restored; the surf frame is fetched first and the pass is skipped when it is not loaded (the
// edge-alpha frame is then never fetched); the edge-alpha frame is fetched next and, when it is not loaded, the surf frame's reference is
// released and nothing is bound or rendered; otherwise stages 0 / 2 / 3 get the sea bump map, the environment map and the surf texture with
// WRAP/WRAP, MIRROR/CLAMP, WRAP/WRAP, user constant 3 = (BumpMatFactor * WaterBumpFactor, 0, 1, 0) and is re-set to (f, 0, -f/2, 1) by
// the setup helper, constant 0 holds the swell factors only when swell is on, pixel constant 5 = clamp(FresnelFactor * 0.25, 0, 1) (0 without
// fresnel), and every patch of the list is rendered; both frame references are released at the end.
#include <cstdio>
#include <cmath>
enum { WRAP = 0, MIRROR = 1, CLAMP = 2 };
struct Frame { bool loaded; int refs; int texture; };
struct Settings { bool swell, fresnel; float swellFactor, bumpMat, fresnelFactor; };
struct Out {
    bool opened, restored, bound, edgeFetched;
    int tex[4], uvU[4], uvV[4];
    float c0x, c3x, c3z, ps5; int patches;
};
static float Limit(float v, float lo, float hi) { return v < lo ? lo : (v > hi ? hi : v); }
static bool GetFrame(Frame& f, int& held) { if (!f.loaded) return false; ++f.refs; held = 1; return true; }
static void Render(Frame& surf, Frame& edge, int bump, int env, float waterBump, const Settings& s, int n, Out& o)
{
    o = Out();
    o.opened = true;
    int surfHeld = 0, edgeHeld = 0;
    bool ok = false;
    if (GetFrame(surf, surfHeld)) {
        o.edgeFetched = true;
        if (GetFrame(edge, edgeHeld))
            ok = true;
    }
    if (ok) {
        o.bound = true;
        o.tex[0] = bump; o.tex[2] = env; o.tex[3] = surf.texture;
        o.uvU[0] = WRAP; o.uvV[0] = WRAP; o.uvU[2] = MIRROR; o.uvV[2] = CLAMP; o.uvU[3] = WRAP; o.uvV[3] = WRAP;
        float f = s.bumpMat * waterBump;
        o.c3x = f; o.c3z = 1.0f;                                         // row: (f, 0, 1, 0)
        o.c0x = s.swell ? s.swellFactor : 0.0f;
        o.c3x = f; o.c3z = f * -0.5f;                                    // setup helper: (f, 0, -f/2, 1)
        o.ps5 = s.fresnel ? Limit(s.fresnelFactor * 0.25f, 0.0f, 1.0f) : 0.0f;
        for (int i = 0; i < n; ++i)
            ++o.patches;
    }
    if (edgeHeld) --edge.refs;                                           // ~CGraphicFrame edge_alpha
    if (surfHeld) --surf.refs;                                           // ~CGraphicFrame surf
    o.restored = true;
}
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-6f; }
int main()
{
    Settings s = { true, true, 0.7f, 0.5f, 2.0f };
    Frame surf = { true, 1, 33 }, edge = { true, 1, 44 };
    Out o;
    Render(surf, edge, 11, 22, 0.8f, s, 3, o);
    bool ok = o.opened && o.restored && o.bound && o.tex[0] == 11 && o.tex[2] == 22 && o.tex[3] == 33 && o.tex[1] == 0
        && o.uvU[0] == WRAP && o.uvV[0] == WRAP && o.uvU[2] == MIRROR && o.uvV[2] == CLAMP && o.uvU[3] == WRAP && o.uvV[3] == WRAP
        && Near(o.c3x, 0.4f) && Near(o.c3z, -0.2f) && Near(o.c0x, 0.7f) && Near(o.ps5, 0.5f) && o.patches == 3
        && surf.refs == 1 && edge.refs == 1;
    Settings h = { false, true, 0.7f, 1.0f, 9.0f };                    // no swell; fresnel clamped at 1
    Render(surf, edge, 11, 22, 0.25f, h, 0, o);
    ok = ok && o.bound && Near(o.c0x, 0.0f) && Near(o.ps5, 1.0f) && Near(o.c3x, 0.25f) && o.patches == 0;
    Settings l = { true, false, 0.1f, 1.0f, -3.0f };                   // fresnel off -> 0
    Render(surf, edge, 11, 22, 1.0f, l, 1, o);
    ok = ok && Near(o.ps5, 0.0f) && o.patches == 1;
    edge.loaded = false;                                                // edge frame missing: surf released, nothing bound
    Render(surf, edge, 11, 22, 1.0f, s, 3, o);
    ok = ok && o.opened && o.restored && o.edgeFetched && !o.bound && o.patches == 0 && o.tex[3] == 0 && surf.refs == 1 && edge.refs == 1;
    surf.loaded = false; edge.loaded = true;                           // surf frame missing: edge never fetched
    Render(surf, edge, 11, 22, 1.0f, s, 3, o);
    ok = ok && o.opened && o.restored && !o.edgeFetched && !o.bound && o.patches == 0 && surf.refs == 1 && edge.refs == 1;
    printf(ok ? "00b77370_TEST PASS\n" : "00b77370_TEST FAIL\n");
    return ok ? 0 : 1;
}