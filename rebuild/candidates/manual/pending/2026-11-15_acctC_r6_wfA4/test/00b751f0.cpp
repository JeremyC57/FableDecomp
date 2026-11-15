// Behaviour test for CEngineWaterRenderer::RenderIceForeground (0x00B751F0): a model of the ice foreground pass -- the capture
// block is always opened and restored; the pass is skipped when the environment's ice texture frame is not loaded; otherwise the
// three stages (bump, environment, ice) get WRAP/WRAP, MIRROR/CLAMP, WRAP/WRAP, the shadow-function states are set only when a shadow
// technique is active, user constant 3 = BumpMatFactor * 0.5 (0 without bump mapping), pixel constant 5 = clamp(FresnelFactor * 0.25,
// 0, 1) (0 without fresnel), specular is enabled, and each patch gets the pixel / vertex shader of the technique for its light count
// before it is rendered; the point lights are disabled after the loop.
#include <cstdio>
#include <cmath>
enum { WRAP = 0, MIRROR = 1, CLAMP = 2 };
struct Settings { bool bump, fresnel; float bumpFactor, fresnelFactor, softEdge; };
struct Out {
    bool opened, restored, rendered, shadowStates, specular, lightsOff;
    int uvU[3], uvV[3]; float c1, c3, ps5; int patches, ps[4], vs[4];
};
static float Limit(float v, float lo, float hi) { return v < lo ? lo : (v > hi ? hi : v); }
static void Render(bool loaded, int technique, const Settings& s, const int* lightCounts, int n, Out& o)
{
    o = Out();
    o.opened = true;
    if (loaded) {
        o.rendered = true;
        o.uvU[0] = WRAP; o.uvV[0] = WRAP; o.uvU[1] = MIRROR; o.uvV[1] = CLAMP; o.uvU[2] = WRAP; o.uvV[2] = WRAP;
        o.shadowStates = technique != 0;
        o.c1 = s.softEdge;
        o.c3 = s.bump ? s.bumpFactor * 0.5f : 0.0f;
        o.ps5 = s.fresnel ? Limit(s.fresnelFactor * 0.25f, 0.0f, 1.0f) : 0.0f;
        o.specular = true;
        for (int i = 0; i < n; ++i) {
            int lights = lightCounts[i] > 3 ? 3 : lightCounts[i];        // GetLightsInArea(area, 3, ...)
            o.ps[i] = technique;                                         // ice / shadowed / colour-buffer pixel shader
            o.vs[i] = technique * 4 + lights;                            // Get*VertexShader(LightCount)
            ++o.patches;
        }
        o.lightsOff = true;
    }
    o.restored = true;
}
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-6f; }
int main()
{
    Settings s = { true, true, 0.8f, 2.0f, 0.3f };
    int counts[3] = { 0, 2, 5 };
    Out o;
    Render(true, 0, s, counts, 3, o);
    bool ok = o.opened && o.restored && o.rendered && !o.shadowStates && o.specular && o.lightsOff
        && o.uvU[1] == MIRROR && o.uvV[1] == CLAMP && o.uvU[2] == WRAP && Near(o.c1, 0.3f) && Near(o.c3, 0.4f) && Near(o.ps5, 0.5f)
        && o.patches == 3 && o.ps[2] == 0 && o.vs[0] == 0 && o.vs[1] == 2 && o.vs[2] == 3;
    Settings h = { false, true, 0.8f, 9.0f, 0.0f };                   // no bump map; fresnel clamped at 1
    Render(true, 2, h, counts, 2, o);
    ok = ok && o.shadowStates && Near(o.c3, 0.0f) && Near(o.ps5, 1.0f) && o.ps[1] == 2 && o.vs[1] == 10 && o.patches == 2;
    Settings l = { true, false, 0.2f, -4.0f, 0.0f };                  // fresnel off -> 0
    Render(true, 1, l, counts, 0, o);
    ok = ok && o.shadowStates && Near(o.ps5, 0.0f) && Near(o.c3, 0.1f) && o.patches == 0 && o.lightsOff;
    Render(false, 1, s, counts, 3, o);                                // frame not loaded: nothing but the capture block
    ok = ok && o.opened && o.restored && !o.rendered && !o.shadowStates && !o.specular && !o.lightsOff && o.patches == 0;
    printf(ok ? "00b751f0_TEST PASS\n" : "00b751f0_TEST FAIL\n");
    return ok ? 0 : 1;
}
