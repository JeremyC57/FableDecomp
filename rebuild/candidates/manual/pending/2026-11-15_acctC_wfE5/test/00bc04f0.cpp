// Behaviour test for 0x00BC04F0 CEnginePrimitiveManagerParticleSpriteGroup::RenderSingleSprite.
// A copy of the row's logic over plain models (the managers record what the row hands them), checked against independently written
// expectations: the three early exits (sprites disabled, bank entry missing, frame 0 not available), the displacement pass (only when the
// displacement renderer is enabled; else the normal path), the alpha-tested displacement-stencil pass (alpha ref 16, GREATER), the normal
// pass (blend op / blend mode, premultiplied flag, the light count from the particle group / local lights / none + 100% self
// illumination), the vertex-shader family (3D face-me, crossed sprite with culling off, billboard) and the point-light index clamp, the
// pixel shader (stipple pair for particles), the three axis constants and the per-sprite constants (only for a visible sprite whose frame
// is available; colour premultiplied by alpha for the additive modes).
#include <cstdio>
#include <cmath>

enum { VS_3D = 1, VS_CROSSED, VS_BILLBOARD };
enum { PS_DISPLACEMENT = 1, PS_NORMAL, PS_STIPPLE_PAIR };
struct Sprite {
    unsigned long bank; float px, py, pz; float sx, sy, ofx, ofy, radius, angle; long blend_mode, blend_op;
    unsigned char r, g, b, a; bool lit, sprite3d, centre, zaxis, particle; unsigned long frame;
};
struct World {
    bool enabled, displacement_on; bool entry_exists, frame0_ok, frame_ok;
    long group_lights, local_lights; unsigned long shader_count;
};
struct Out {
    bool rendered, drawn; long blend_op, src, dst; bool alpha_test; long alpha_ref; bool cull_none;
    int vs_family; unsigned long vs_index; int ps; long self_illum; long lights;
    float c[3][4]; float col[4]; float pos[4]; float shape[4];
};

static void RenderSingleSprite(const World& w, const Sprite& s, int pass, const float view[3][3], float wf, float hf, Out& o)
{
    o.rendered = o.drawn = false; o.alpha_test = false; o.cull_none = false; o.self_illum = -1; o.lights = 0;
    if (!w.enabled || !w.entry_exists || !w.frame0_ok)
        return;
    o.rendered = true;
    bool additive = false;
    if ((pass == 6 && w.displacement_on) || pass == 7) {
        if (pass == 6) { o.blend_op = 1; o.src = 5; o.dst = 6; }
        else { o.alpha_test = true; o.alpha_ref = 16; }
        if (s.sprite3d) o.vs_family = VS_3D;
        else if (s.zaxis) { o.cull_none = true; o.vs_family = VS_CROSSED; }
        else o.vs_family = VS_BILLBOARD;
        o.vs_index = 0;
        o.ps = PS_DISPLACEMENT;
    } else {
        o.blend_op = s.blend_op == 1 ? 2 : s.blend_op == 2 ? 3 : 1;
        if (s.blend_mode == 3) { o.src = 2; o.dst = 2; additive = true; }
        else if (s.blend_mode == 4) { o.src = 2; o.dst = 4; additive = true; }
        else { o.src = 5; o.dst = 6; }
        unsigned long lights = 0;
        if (s.lit) lights = s.particle ? w.group_lights : w.local_lights;
        else o.self_illum = 100;
        o.lights = (long)lights;
        if (s.sprite3d) o.vs_family = VS_3D;
        else if (s.zaxis) { o.cull_none = true; o.vs_family = VS_CROSSED; }
        else o.vs_family = VS_BILLBOARD;
        o.vs_index = lights > w.shader_count - 1 ? w.shader_count - 1 : lights;
        o.ps = s.particle ? PS_STIPPLE_PAIR : PS_NORMAL;
    }
    o.c[0][0] = view[0][0]; o.c[0][1] = view[1][0]; o.c[0][2] = s.ofx; o.c[0][3] = s.ofy;
    o.c[1][0] = view[0][1]; o.c[1][1] = view[1][1]; o.c[1][2] = wf; o.c[1][3] = hf;
    o.c[2][0] = view[0][2]; o.c[2][1] = view[1][2]; o.c[2][2] = s.centre ? 1.0f : 0.0f; o.c[2][3] = 0.0f;
    if (s.a > 0 && s.sx > 0.0f && s.sy > 0.0f && w.frame_ok) {
        float r = s.r / 255.0f, g = s.g / 255.0f, b = s.b / 255.0f, a = s.a / 255.0f;
        float scale = additive ? a : 1.0f;
        o.col[0] = r * scale; o.col[1] = g * scale; o.col[2] = b * scale; o.col[3] = a;
        o.pos[0] = s.px; o.pos[1] = s.py; o.pos[2] = s.pz; o.pos[3] = 0.0f;
        o.shape[0] = std::cos(s.angle * 6.2831853f); o.shape[1] = std::sin(s.angle * 6.2831853f); o.shape[2] = s.sx; o.shape[3] = s.sy;
        o.drawn = true;
    }
}

static int fails;
static void Check(bool c, const char* what) { if (!c) { ++fails; std::printf("FAIL %s\n", what); } }
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-3f; }

int main()
{
    const float view[3][3] = { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 } };
    World w = { true, true, true, true, true, 2, 7, 4 };
    Sprite s = { 5, 1.0f, 2.0f, 3.0f, 0.5f, 0.25f, 0.1f, 0.2f, 1.0f, 0.25f, 3, 0, 255, 102, 0, 51,
                 true, false, true, false, false, 9 };
    Out o;

    World e = w; e.enabled = false; RenderSingleSprite(e, s, 0, view, 0.5f, 0.25f, o); Check(!o.rendered, "disabled");
    e = w; e.entry_exists = false; RenderSingleSprite(e, s, 0, view, 0.5f, 0.25f, o); Check(!o.rendered, "no entry");
    e = w; e.frame0_ok = false; RenderSingleSprite(e, s, 0, view, 0.5f, 0.25f, o); Check(!o.rendered, "no frame");

    // Normal pass, additive, lit local lights 7 clamped to shader 3, billboard.
    RenderSingleSprite(w, s, 0, view, 0.5f, 0.25f, o);
    Check(o.rendered && o.drawn, "normal drawn");
    Check(o.blend_op == 1 && o.src == 2 && o.dst == 2, "additive blend");
    Check(o.lights == 7 && o.vs_index == 3 && o.vs_family == VS_BILLBOARD && !o.cull_none && o.ps == PS_NORMAL, "lights / shaders");
    Check(o.c[0][0] == 1 && o.c[0][1] == 4 && o.c[1][0] == 2 && o.c[1][1] == 5 && o.c[2][0] == 3 && o.c[2][1] == 6, "axes");
    Check(o.c[0][2] == 0.1f && o.c[0][3] == 0.2f && o.c[1][2] == 0.5f && o.c[1][3] == 0.25f && o.c[2][2] == 1.0f && o.c[2][3] == 0.0f, "factors");
    Check(Near(o.col[0], 0.2f) && Near(o.col[1], 0.08f) && Near(o.col[2], 0.0f) && Near(o.col[3], 0.2f), "premultiplied colour");
    Check(Near(o.shape[0], 0.0f) && Near(o.shape[1], 1.0f) && o.shape[2] == 0.5f && o.pos[2] == 3.0f, "shape / position");

    // Particle: group lights, stipple pair, crossed sprite culling off; alpha mode keeps the colour.
    Sprite p = s; p.particle = true; p.zaxis = true; p.blend_mode = 2; p.blend_op = 2; p.centre = false;
    RenderSingleSprite(w, p, 0, view, 0.5f, 0.25f, o);
    Check(o.lights == 2 && o.vs_index == 2 && o.vs_family == VS_CROSSED && o.cull_none && o.ps == PS_STIPPLE_PAIR, "particle");
    Check(o.blend_op == 3 && o.src == 5 && o.dst == 6 && Near(o.col[0], 1.0f) && Near(o.col[1], 0.4f) && o.c[2][2] == 0.0f, "alpha blend");

    // Unlit 3D: self illumination, light index 0.
    Sprite u = s; u.lit = false; u.sprite3d = true; u.blend_mode = 4; u.blend_op = 1;
    RenderSingleSprite(w, u, 0, view, 0.5f, 0.25f, o);
    Check(o.self_illum == 100 && o.lights == 0 && o.vs_index == 0 && o.vs_family == VS_3D && o.dst == 4 && o.blend_op == 2, "unlit");

    // Displacement pass on / off, stencil pass.
    RenderSingleSprite(w, s, 6, view, 0.5f, 0.25f, o);
    Check(o.ps == PS_DISPLACEMENT && o.src == 5 && Near(o.col[0], 1.0f), "displacement pass");
    e = w; e.displacement_on = false; RenderSingleSprite(e, s, 6, view, 0.5f, 0.25f, o);
    Check(o.ps == PS_NORMAL && o.src == 2, "displacement off -> normal");
    RenderSingleSprite(w, s, 7, view, 0.5f, 0.25f, o);
    Check(o.alpha_test && o.alpha_ref == 16 && o.ps == PS_DISPLACEMENT, "stencil pass");

    // Invisible sprite / second frame missing: axes set, nothing drawn.
    Sprite z = s; z.a = 0; RenderSingleSprite(w, z, 0, view, 0.5f, 0.25f, o); Check(o.rendered && !o.drawn, "alpha 0");
    z = s; z.sy = 0.0f; RenderSingleSprite(w, z, 0, view, 0.5f, 0.25f, o); Check(!o.drawn, "size 0");
    e = w; e.frame_ok = false; RenderSingleSprite(e, s, 0, view, 0.5f, 0.25f, o); Check(!o.drawn, "frame missing");

    if (!fails) std::printf("00bc04f0_TEST PASS\n");
    return fails;
}
