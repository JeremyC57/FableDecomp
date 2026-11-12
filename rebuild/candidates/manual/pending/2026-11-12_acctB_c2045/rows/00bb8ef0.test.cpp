// Behaviour test for 0x00BB8EF0 CEnginePrimitiveManagerParticleDecalGroup::RenderSubPrimitive.
// A copy of the row's logic over plain models (the render managers record their calls into one event log; the graphic bank is an array of
// infos whose GetFrame hands out a counted frame; the lighting / shader / render-state managers keep the state the inline setters touch),
// checked against independently written expectations: the four early exits (disabled manager, empty group, mesh-decal group, frame not
// available -- the frame's reference released, nothing else touched), the full pass for every blend op / blend mode / lighting case (state
// values, the premultiplied flag and the point-light count handed to RenderLandscapeDecalGroup, the order of the calls), and the scoped
// restores at the end (vertex-shader constant layout, shader enables, capture block, frame release).
#include <cstdio>

enum { E_GETFRAME = 1, E_CAPTURE, E_VS_ON, E_PS_ON, E_TEXTURE, E_UVU, E_UVV, E_BLENDOP, E_SRCBLEND, E_DSTBLEND, E_POINTLIGHTS,
       E_SELFILLUM, E_GLOBALLIGHT, E_PREPARE, E_LAYOUT, E_LANDSCAPE, E_RESTOREDEFAULT, E_VS_OFF, E_PS_OFF, E_RESTORECAPTURE,
       E_FRAMERELEASE };
enum { MAXLOG = 64 };
struct Log {
    int ev[MAXLOG]; long arg[MAXLOG]; int n;
    void Add(int e, long a) { if (n < MAXLOG) { ev[n] = e; arg[n] = a; ++n; } }
};

// D3D values the engine tables hold (CLAMP 3; BLENDOP ADD 1 / SUBTRACT 2 / REVSUBTRACT 3; BLEND ONE 2, INVSRCCOLOR 4, SRCALPHA 5, INVSRCALPHA 6).
static const long UV_CLAMP = 3;
static const long BLENDOP_ADD = 1, BLENDOP_SUB = 2, BLENDOP_REVSUB = 3;
static const long BLEND_ONE = 2, BLEND_INVSRCCOLOR = 4, BLEND_SRCALPHA = 5, BLEND_INVSRCALPHA = 6;

struct FrameData { long refs; long texture; };
struct Frame { FrameData* data; };
struct GraphicInfo { bool available; FrameData* data; };

struct World {
    Log log;
    bool vs_enabled, ps_enabled;          // CShaderRenderManager enables
    long layout;                          // current vertex-shader constant layout
    long self_illum; bool faded; unsigned char channel[3];   // CEngineLightingManager primitive parameters
    long point_lights;                    // what SetUpPointLights returns
    long render_premultiplied, render_lights;
};

struct DecalGroup {
    int primitive_count; unsigned long bank_index; unsigned short flags; long blend_mode; long blend_op; bool landscape;
};

static bool GetFrame(World& w, const GraphicInfo& info, Frame& frame)
{
    w.log.Add(E_GETFRAME, 0);
    if (!info.available) return false;
    frame.data = info.data; ++frame.data->refs;
    return true;
}
static void ReleaseFrame(World& w, Frame& frame)
{
    if (frame.data) { --frame.data->refs; w.log.Add(E_FRAMERELEASE, frame.data->refs); frame.data = 0; }
}
static void SetShaders(World& w, bool vs, bool ps)
{
    if (w.vs_enabled != vs) { w.vs_enabled = vs; w.log.Add(vs ? E_VS_ON : E_VS_OFF, 0); }
    if (w.ps_enabled != ps) { w.ps_enabled = ps; w.log.Add(ps ? E_PS_ON : E_PS_OFF, 0); }
}
static void SetSelfIllum(World& w, long v) { if (w.self_illum != v) { w.self_illum = v; w.log.Add(E_SELFILLUM, v); } }

// The row's logic.
static void RenderSubPrimitive(World& w, bool enabled, const DecalGroup& group, const GraphicInfo* bank, unsigned long bank_size)
{
    if (!enabled)
        return;
    if (group.primitive_count > 0 && group.landscape) {
        Frame frame; frame.data = 0;
        const GraphicInfo* info = group.bank_index < bank_size ? &bank[group.bank_index] : 0;
        if (!GetFrame(w, *info, frame)) { ReleaseFrame(w, frame); return; }
        w.log.Add(E_CAPTURE, 0);
        bool old_vs = w.vs_enabled, old_ps = w.ps_enabled;
        SetShaders(w, true, true);
        w.log.Add(E_TEXTURE, frame.data->texture);
        w.log.Add(E_UVU, UV_CLAMP);
        w.log.Add(E_UVV, UV_CLAMP);
        w.log.Add(E_BLENDOP, group.blend_op == 1 ? BLENDOP_SUB : group.blend_op == 2 ? BLENDOP_REVSUB : BLENDOP_ADD);
        bool premultiplied;
        if (group.blend_mode == 3) { w.log.Add(E_SRCBLEND, BLEND_ONE); w.log.Add(E_DSTBLEND, BLEND_ONE); premultiplied = true; }
        else if (group.blend_mode == 4) { w.log.Add(E_SRCBLEND, BLEND_ONE); w.log.Add(E_DSTBLEND, BLEND_INVSRCCOLOR); premultiplied = true; }
        else { w.log.Add(E_SRCBLEND, BLEND_SRCALPHA); w.log.Add(E_DSTBLEND, BLEND_INVSRCALPHA); premultiplied = false; }
        long lights = 0;
        if (group.flags & 0x80) { lights = w.point_lights; w.log.Add(E_POINTLIGHTS, lights); }
        else SetSelfIllum(w, 100);
        w.log.Add(E_GLOBALLIGHT, 0);
        w.log.Add(E_PREPARE, 1);
        long old_layout = w.layout; w.layout = 3; w.log.Add(E_LAYOUT, 3);
        if (group.landscape) { w.render_premultiplied = premultiplied; w.render_lights = lights; w.log.Add(E_LANDSCAPE, lights); }
        w.faded = false; w.channel[0] = w.channel[1] = w.channel[2] = 0; SetSelfIllum(w, 0); w.log.Add(E_RESTOREDEFAULT, 0);
        w.layout = old_layout; w.log.Add(E_LAYOUT, old_layout);
        SetShaders(w, old_vs, old_ps);
        w.log.Add(E_RESTORECAPTURE, 0);
        ReleaseFrame(w, frame);
    }
}

static int failures = 0;
static void Check(bool ok, int line) { if (!ok) { std::printf("check failed at line %d\n", line); ++failures; } }
#define CHECK(x) Check((x), __LINE__)

static void Reset(World& w)
{
    w.log.n = 0; w.vs_enabled = false; w.ps_enabled = true; w.layout = 7; w.self_illum = 0; w.faded = true;
    w.channel[0] = 1; w.channel[1] = 2; w.channel[2] = 3; w.point_lights = 4; w.render_premultiplied = -1; w.render_lights = -1;
}
static bool Seq(const World& w, const int* ev, int n)
{
    if (w.log.n != n) return false;
    for (int i = 0; i < n; ++i) if (w.log.ev[i] != ev[i]) return false;
    return true;
}
static long ArgOf(const World& w, int e, int nth)
{
    for (int i = 0; i < w.log.n; ++i) if (w.log.ev[i] == e && nth-- == 0) return w.log.arg[i];
    return -999;
}

int main()
{
    World w;
    FrameData data; data.refs = 1; data.texture = 0x1234;
    GraphicInfo bank[3];
    bank[0].available = true; bank[0].data = &data;
    bank[1].available = false; bank[1].data = &data;
    bank[2].available = true; bank[2].data = &data;
    DecalGroup g; g.primitive_count = 2; g.bank_index = 2; g.flags = 0; g.blend_mode = 0; g.blend_op = 0; g.landscape = true;

    // Early exits: disabled manager, empty group, mesh-decal group: nothing at all.
    Reset(w); RenderSubPrimitive(w, false, g, bank, 3); CHECK(w.log.n == 0);
    DecalGroup e = g; e.primitive_count = 0;
    Reset(w); RenderSubPrimitive(w, true, e, bank, 3); CHECK(w.log.n == 0);
    DecalGroup m = g; m.landscape = false;
    Reset(w); RenderSubPrimitive(w, true, m, bank, 3); CHECK(w.log.n == 0);
    // Frame not available: only the fetch, no state change, nothing to release.
    DecalGroup u = g; u.bank_index = 1;
    Reset(w); RenderSubPrimitive(w, true, u, bank, 3);
    { const int ev[] = { E_GETFRAME }; CHECK(Seq(w, ev, 1)); }
    CHECK(data.refs == 1 && w.layout == 7 && !w.vs_enabled && w.ps_enabled);

    // Full pass, default blend, unlit: self illumination 100 then back to 0.
    Reset(w); RenderSubPrimitive(w, true, g, bank, 3);
    {
        const int ev[] = { E_GETFRAME, E_CAPTURE, E_VS_ON, E_TEXTURE, E_UVU, E_UVV, E_BLENDOP, E_SRCBLEND, E_DSTBLEND, E_SELFILLUM,
                           E_GLOBALLIGHT, E_PREPARE, E_LAYOUT, E_LANDSCAPE, E_SELFILLUM, E_RESTOREDEFAULT, E_LAYOUT, E_VS_OFF,
                           E_RESTORECAPTURE, E_FRAMERELEASE };
        CHECK(Seq(w, ev, 20));
    }
    CHECK(ArgOf(w, E_TEXTURE, 0) == 0x1234 && ArgOf(w, E_UVU, 0) == UV_CLAMP && ArgOf(w, E_UVV, 0) == UV_CLAMP);
    CHECK(ArgOf(w, E_BLENDOP, 0) == BLENDOP_ADD && ArgOf(w, E_SRCBLEND, 0) == BLEND_SRCALPHA && ArgOf(w, E_DSTBLEND, 0) == BLEND_INVSRCALPHA);
    CHECK(ArgOf(w, E_SELFILLUM, 0) == 100 && ArgOf(w, E_SELFILLUM, 1) == 0 && ArgOf(w, E_PREPARE, 0) == 1);
    CHECK(ArgOf(w, E_LAYOUT, 0) == 3 && ArgOf(w, E_LAYOUT, 1) == 7 && w.layout == 7);
    CHECK(w.render_premultiplied == 0 && w.render_lights == 0 && ArgOf(w, E_FRAMERELEASE, 0) == 1 && data.refs == 1);
    CHECK(!w.vs_enabled && w.ps_enabled && !w.faded && w.channel[0] == 0 && w.channel[1] == 0 && w.channel[2] == 0);

    // Lit group, additive (mode 3), blend op 1: point lights handed on, premultiplied, no self illumination.
    DecalGroup a = g; a.flags = 0x80; a.blend_mode = 3; a.blend_op = 1;
    Reset(w); RenderSubPrimitive(w, true, a, bank, 3);
    CHECK(ArgOf(w, E_POINTLIGHTS, 0) == 4 && ArgOf(w, E_SELFILLUM, 0) == -999);
    CHECK(ArgOf(w, E_BLENDOP, 0) == BLENDOP_SUB && ArgOf(w, E_SRCBLEND, 0) == BLEND_ONE && ArgOf(w, E_DSTBLEND, 0) == BLEND_ONE);
    CHECK(w.render_premultiplied == 1 && w.render_lights == 4 && data.refs == 1);
    // Add-smooth (mode 4), blend op 2, flags without bit 7.
    DecalGroup s = g; s.flags = 0x7f; s.blend_mode = 4; s.blend_op = 2;
    Reset(w); RenderSubPrimitive(w, true, s, bank, 3);
    CHECK(ArgOf(w, E_BLENDOP, 0) == BLENDOP_REVSUB && ArgOf(w, E_SRCBLEND, 0) == BLEND_ONE && ArgOf(w, E_DSTBLEND, 0) == BLEND_INVSRCCOLOR);
    CHECK(w.render_premultiplied == 1 && w.render_lights == 0 && ArgOf(w, E_SELFILLUM, 0) == 100);
    // Other op / mode values fall to the defaults.
    DecalGroup o = g; o.blend_mode = 2; o.blend_op = 3;
    Reset(w); RenderSubPrimitive(w, true, o, bank, 3);
    CHECK(ArgOf(w, E_BLENDOP, 0) == BLENDOP_ADD && w.render_premultiplied == 0);

    if (failures) { std::printf("00bb8ef0_TEST FAIL\n"); return 1; }
    std::printf("00bb8ef0_TEST PASS\n");
    return 0;
}
