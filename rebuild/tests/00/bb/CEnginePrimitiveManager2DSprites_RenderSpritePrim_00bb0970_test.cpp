// Behaviour test for 0x00BB0970 CEnginePrimitiveManager2DSprites::RenderSpritePrim.
// A copy of the row's logic over plain models (the frame bank, the alpha-mask info, the 2D renderer and the sub-primitive renderer record
// what the row hands them), checked against independently written expectations: the viewport / alpha-mask clip rejections, the colour-A and
// layer-enable gates, the GetFrame flags (0x11 for an EnginePrimitiveFlags 0x80 sprite), the frame UVs (half-texel 0.01 / size on U1 U2 V2
// V3, then the interpolated UV offset on every corner), the default 0..1 UVs without a bank index, the linear-texture texel scaling, the mask
// texture (the mask frame's texture, or the saturated texture), the rotated quad in clip space (vertex order TL TR BL BR with UV 0 1 3 2),
// the halved RGB with alpha kept, the queueing (the given type, or displacement buffer + a copied stencil sub-primitive when flag 0x400 is
// set and 2D displacement is on, nothing when it is off) and the debug outline box.
#include <cstdio>
#include <cmath>

struct Vtx { float X, Y, Z; unsigned long Diffuse; float U, V; };
struct Sub { Vtx v[4]; const void* tex; const void* mask; bool filter, wrap; int blend; unsigned char alpha; unsigned long flags; };
struct Sprite {
    float tlx, tly, brx, bry; int layer; unsigned long bank, frame; float start_angle, end_angle; unsigned long colour;   // A in the top byte
    const void* texture; bool filter; float uvs0x, uvs0y, uvs1x, uvs1y; bool wrap; int blend; unsigned long flags; float interpolate;
};
struct World {
    long vp_tlx, vp_tly, vp_brx, vp_bry;
    unsigned long mask_index; bool mask_bank; bool mask_outside; float mtlx, mtly, mbrx, mbry;
    bool layer_enabled; bool frame_ok; bool mask_ok; unsigned long frame_flags_seen, mask_flags_seen;
    const void* frame_tex; float quad_u[4], quad_v[4]; unsigned long fw, fh;
    bool linear; unsigned long tw, th; const void* mask_tex; const void* sat_tex;
    long rt_x, rt_y; float z; bool displacement; bool debug;
    int queued; int types[2]; Sub subs[2]; int boxes; float box[4];
};

static float FastCosRef(float a) { return (float)cos(a * 6.283185307179586); }   // GFFastCos / GFFastSin take turns

static void Render(const Sprite& s, int type, World& w)
{
    w.queued = 0; w.boxes = 0;
    const void* texture = s.texture;
    if (w.vp_brx < s.tlx || w.vp_tlx > s.brx || w.vp_bry < s.tly || w.vp_tly > s.bry) return;
    if (w.mask_index && w.mask_bank && w.mask_outside)
        if (s.tlx > w.mbrx || s.brx < w.mtlx || s.tly > w.mbry || s.bry < w.mtly) return;
    if ((s.colour >> 24) == 0) return;
    if (!w.layer_enabled) return;
    unsigned long flags = (s.flags & 0x80) ? 0x11 : 0;
    float u[4], v[4];
    if (s.bank) {
        w.frame_flags_seen = flags;
        if (!w.frame_ok) return;
        texture = w.frame_tex;
        for (int i = 0; i < 4; ++i) { u[i] = w.quad_u[i]; v[i] = w.quad_v[i]; }
        float du = 0.01f / w.fw, dv = 0.01f / w.fh;
        u[1] += du; u[2] += du; v[2] += dv; v[3] += dv;
        float ox = (s.uvs1x - s.uvs0x) * s.interpolate + s.uvs0x, oy = (s.uvs1y - s.uvs0y) * s.interpolate + s.uvs0y;
        for (int i = 0; i < 4; ++i) { u[i] += ox; v[i] += oy; }
    } else {
        u[0] = 0; v[0] = 0; u[1] = 1; v[1] = 0; u[2] = 1; v[2] = 1; u[3] = 0; v[3] = 1;
    }
    if (texture && w.linear)
        for (int i = 0; i < 4; ++i) { u[i] *= (float)w.tw; v[i] *= (float)w.th; }
    const void* mask;
    if (w.mask_index && w.mask_bank) {
        w.mask_flags_seen = flags;
        if (!w.mask_ok) return;
        mask = w.mask_tex;
    } else mask = w.sat_tex;
    float hw = (s.brx - s.tlx) * 0.5f, hh = (s.bry - s.tly) * 0.5f;
    float angle = (s.end_angle - s.start_angle) * s.interpolate + s.start_angle;
    float c = FastCosRef(angle), sn = FastCosRef(angle - 0.25f);
    float ax = c * hw, ay = -(hw * sn), dx = -(sn * hh), dy = -(c * hh), cx = hw + s.tlx, cy = hh + s.tly;
    float px[4] = { cx - ax + dx, cx + ax + dx, cx - ax - dx, cx + ax - dx };
    float py[4] = { cy - ay + dy, cy + ay + dy, cy - ay - dy, cy + ay - dy };
    float sx = 1.0f / (w.rt_x * 0.5f), sy = 1.0f / (w.rt_y * 0.5f);
    unsigned long colour = ((s.colour >> 1) & 0x7f7f7f) | (s.colour & 0xff000000);
    Sub& sub = w.subs[0];
    static const int uv[4] = { 0, 1, 3, 2 };
    for (int i = 0; i < 4; ++i) {
        sub.v[i].X = px[i] * sx - 1.0f; sub.v[i].Y = 1.0f - py[i] * sy; sub.v[i].Z = w.z; sub.v[i].Diffuse = colour;
        sub.v[i].U = u[uv[i]]; sub.v[i].V = v[uv[i]];
    }
    sub.tex = texture; sub.mask = mask; sub.filter = s.filter; sub.wrap = s.wrap; sub.blend = s.blend; sub.alpha = 0xff; sub.flags = 1;
    if (s.flags & 0x400) {
        if (w.displacement) {
            w.types[w.queued++] = 5;
            Sub& st = w.subs[1];
            for (int i = 0; i < 4; ++i) st.v[i] = sub.v[i];
            st.tex = sub.tex; st.mask = sub.mask; st.filter = sub.filter; st.wrap = sub.wrap; st.blend = sub.blend;
            w.types[w.queued++] = 6;
        }
    } else w.types[w.queued++] = type;
    if (w.debug) { w.boxes = 1; w.box[0] = s.tlx; w.box[1] = s.tly; w.box[2] = s.brx; w.box[3] = s.bry; }
}

static int fails = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL line %d: %s\n", __LINE__, #c); ++fails; } } while (0)
static bool feq(float a, float b) { return std::fabs(a - b) < 1e-4f; }

int main()
{
    int tex = 0, ftex = 0, mtex = 0, sat = 0;
    World w = {};
    w.vp_tlx = 0; w.vp_tly = 0; w.vp_brx = 640; w.vp_bry = 480; w.layer_enabled = true; w.frame_ok = true; w.mask_ok = true;
    w.frame_tex = &ftex; w.mask_tex = &mtex; w.sat_tex = &sat; w.fw = 50; w.fh = 25; w.tw = 256; w.th = 128;
    w.quad_u[0] = 0.25f; w.quad_u[1] = 0.5f; w.quad_u[2] = 0.5f; w.quad_u[3] = 0.25f;
    w.quad_v[0] = 0.125f; w.quad_v[1] = 0.125f; w.quad_v[2] = 0.375f; w.quad_v[3] = 0.375f;
    w.rt_x = 640; w.rt_y = 480; w.z = 0.75f;
    Sprite s = {};
    s.tlx = 100; s.tly = 50; s.brx = 200; s.bry = 150; s.colour = 0x80FF4020; s.texture = &tex; s.filter = true; s.wrap = false; s.blend = 3;
    // untextured bank-less sprite, no rotation: clip-space quad, default UVs, halved colour, queued with the given type
    Render(s, 2, w);
    CHECK(w.queued == 1 && w.types[0] == 2 && w.boxes == 0);
    CHECK(feq(w.subs[0].v[0].X, 100 / 320.0f - 1) && feq(w.subs[0].v[0].Y, 1 - 50 / 240.0f));
    CHECK(feq(w.subs[0].v[1].X, 200 / 320.0f - 1) && feq(w.subs[0].v[2].Y, 1 - 150 / 240.0f) && feq(w.subs[0].v[3].X, 200 / 320.0f - 1));
    CHECK(feq(w.subs[0].v[0].U, 0) && feq(w.subs[0].v[1].U, 1) && feq(w.subs[0].v[2].U, 0) && feq(w.subs[0].v[2].V, 1) && feq(w.subs[0].v[3].U, 1));
    CHECK(w.subs[0].v[3].Diffuse == 0x807F2010 && feq(w.subs[0].v[1].Z, 0.75f) && w.subs[0].tex == &tex && w.subs[0].mask == &sat);
    CHECK(w.subs[0].alpha == 0xff && w.subs[0].flags == 1 && w.subs[0].blend == 3 && w.subs[0].filter && !w.subs[0].wrap);
    // linear texture: texel UVs
    w.linear = true; Render(s, 2, w);
    CHECK(feq(w.subs[0].v[1].U, 256) && feq(w.subs[0].v[3].V, 128) && feq(w.subs[0].v[0].V, 0));
    w.linear = false;
    // rejections: outside the viewport, outside the mask box, zero alpha, disabled layer
    s.tlx = 700; s.brx = 800; Render(s, 2, w); CHECK(w.queued == 0);
    s.tlx = 100; s.brx = 200; s.bry = -1; s.tly = -10; Render(s, 2, w); CHECK(w.queued == 0);
    s.tly = 50; s.bry = 150;
    w.mask_index = 7; w.mask_bank = true; w.mask_outside = true; w.mtlx = 300; w.mtly = 0; w.mbrx = 400; w.mbry = 400;
    Render(s, 2, w); CHECK(w.queued == 0);
    w.mask_outside = false; Render(s, 2, w); CHECK(w.queued == 1 && w.subs[0].mask == &mtex);
    w.mask_ok = false; Render(s, 2, w); CHECK(w.queued == 0); w.mask_ok = true;
    s.colour = 0x00FFFFFF; Render(s, 2, w); CHECK(w.queued == 0); s.colour = 0xFF0000FF;
    w.layer_enabled = false; Render(s, 2, w); CHECK(w.queued == 0); w.layer_enabled = true;
    // bank frame: half-texel nudge, interpolated UV offset, GetFrame flags, frame texture
    s.bank = 3; s.flags = 0x80; s.interpolate = 0.5f; s.uvs0x = 0.0f; s.uvs0y = 0.5f; s.uvs1x = 1.0f; s.uvs1y = 1.5f;
    Render(s, 1, w);
    CHECK(w.frame_flags_seen == 0x11 && w.mask_flags_seen == 0x11 && w.subs[0].tex == &ftex);
    CHECK(feq(w.subs[0].v[0].U, 0.75f) && feq(w.subs[0].v[0].V, 1.125f) && feq(w.subs[0].v[1].U, 1.0f + 0.0002f));
    CHECK(feq(w.subs[0].v[2].U, 0.75f) && feq(w.subs[0].v[2].V, 1.375f + 0.0004f) && feq(w.subs[0].v[3].U, 1.0f + 0.0002f));
    w.frame_ok = false; Render(s, 1, w); CHECK(w.queued == 0); w.frame_ok = true;
    // rotation by a quarter turn about the centre (150, 100)
    s.bank = 0; s.flags = 0; s.start_angle = 0.25f; s.end_angle = 0.25f; w.mask_index = 0;
    Render(s, 1, w);
    CHECK(feq(w.subs[0].v[0].X, 100 / 320.0f - 1) && feq(w.subs[0].v[0].Y, 1 - 150 / 240.0f) && w.subs[0].mask == &sat);
    // displacement: buffer + copied stencil when enabled, nothing when disabled; debug box
    s.start_angle = s.end_angle = 0; s.flags = 0x400; w.displacement = true; w.debug = true;
    Render(s, 1, w);
    CHECK(w.queued == 2 && w.types[0] == 5 && w.types[1] == 6 && feq(w.subs[1].v[3].Y, w.subs[0].v[3].Y) && w.subs[1].tex == w.subs[0].tex);
    CHECK(w.boxes == 1 && feq(w.box[2], 200));
    w.displacement = false; Render(s, 1, w); CHECK(w.queued == 0 && w.boxes == 1);
    if (fails == 0) std::printf("00bb0970_TEST PASS\n");
    return fails;
}