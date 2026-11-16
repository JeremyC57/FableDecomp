// Behaviour test for 0x00BAFA00 CEnginePrimitiveManager2DSprites::RenderClockPrim.
// A copy of the row's logic over plain models (the viewport, the alpha-mask info, the 2D layer enables and the sub-primitive renderer record
// what the row hands them), checked against independently written expectations: the viewport / alpha-mask clip rejections, the colour-A
// and layer-enable gates, the clamped interpolated angle, the fraction rules (0 / 1 at the ends, eighths near the top, quarters elsewhere),
// the segment (ceil(angle * 4 + 0.5), wrapping 5 -> 1), the seventh vertex interpolated between the segment's corner and the next one, the
// clock-face index list (three indices per corner, the split corner writing two triangles and recording the index offset), the colours /
// textures swapped into the sub-primitive, the queue type (displacement buffer when flag 0x400 is set, the given type otherwise) and the
// debug outline box.
#include <cstdio>
#include <cmath>

struct Vtx { float X, Y, U, V; };
struct Sub { Vtx v[7]; unsigned short idx[32]; int index_offset; unsigned long colour, colour2; int tex0, tex1; };
struct Clock {
    long tlx, tly, brx, bry; unsigned long colour, colour2; int layer; float angle_start, angle_end, interpolate;
    unsigned long flags; int tex_a, tex_b;
};
struct World {
    long vp_tlx, vp_tly, vp_brx, vp_bry;
    bool mask_bank; bool mask_outside; long mtlx, mtly, mbrx, mbry;
    bool layer_enabled; bool debug;
    int queued; int type; Sub sub; int boxes; int count;
};

static long CeilLong(float value)                  // GFFloatToLongCeil: round-to-nearest of value + 0.5, minus one when exact
{
    double r = (double)value + 0.5;
    long result = (long)floor(r + 0.5);
    if (r + 0.5 == floor(r + 0.5) && ((long)floor(r + 0.5) & 1))   // round half to even, as fistp does
        result -= 1;
    float back = (float)((double)result - 1.0);
    if (value == back)
        result -= 1;
    return result;
}

static void Render(const Clock& c, int type, World& w)
{
    w.queued = 0; w.boxes = 0; w.count = 0;
    if (w.vp_brx < c.tlx || w.vp_tlx > c.brx || w.vp_bry < c.tly || w.vp_tly > c.bry)
        return;
    if (w.mask_bank && w.mask_outside) {
        if (c.tlx > w.mbrx || c.brx < w.mtlx || c.tly > w.mbry || c.bry < w.mtly)
            return;
    }
    if ((c.colour >> 24) == 0)
        return;
    if (!w.layer_enabled)
        return;
    Sub& s = w.sub;
    // fan corners: centre, top middle, TR, BR, BL, TL (clip-space conversion is linear; plain box coordinates are enough here)
    float cx = (c.brx - c.tlx) * 0.5f + c.tlx, cy = (c.bry - c.tly) * 0.5f + c.tly;
    float xs[6] = { cx, cx, (float)c.brx, (float)c.brx, (float)c.tlx, (float)c.tlx };
    float ys[6] = { cy, (float)c.tly, (float)c.tly, (float)c.bry, (float)c.bry, (float)c.tly };
    float us[6] = { 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f };
    float vs[6] = { 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f };
    for (int k = 0; k < 6; ++k) { s.v[k].X = xs[k]; s.v[k].Y = ys[k]; s.v[k].U = us[k]; s.v[k].V = vs[k]; }

    float angle = (c.angle_end - c.angle_start) * c.interpolate + c.angle_start;
    angle = angle < 0.0f ? 0.0f : (angle > 1.0f ? 1.0f : angle);
    float fraction;
    if (angle <= 0.0 || angle >= 1.0f)
        fraction = angle < 0.0f ? 0.0f : (angle > 1.0f ? 1.0f : angle);
    else if (angle < 0.125f || angle > 0.875f)
        fraction = (float)fmod((double)angle, 0.125) * 8.0f;
    else
        fraction = (float)fmod((double)(angle - 0.125f), 0.25) * 4.0f;
    long segment = CeilLong(angle * 4.0f + 0.5f);
    long next = segment == 5 ? 1 : segment + 1;
    const Vtx& to = s.v[next];
    const Vtx& from = s.v[segment];
    s.v[6].X = (to.X - from.X) * fraction + from.X;
    s.v[6].Y = (to.Y - from.Y) * fraction + from.Y;
    s.v[6].U = (to.U - from.U) * fraction + from.U;
    s.v[6].V = (to.V - from.V) * fraction + from.V;
    long index = 0;
    for (long i = 1; i < 6; ++i) {
        long following = i == 5 ? 1 : i + 1;
        if (segment == i) {
            s.idx[index++] = 0;
            s.idx[index++] = (unsigned short)i;
            s.idx[index++] = 6;
            s.index_offset = (unsigned char)index;
            s.idx[index++] = 0;
            s.idx[index++] = 6;
            s.idx[index++] = (unsigned short)following;
        } else {
            s.idx[index++] = 0;
            s.idx[index++] = (unsigned short)i;
            s.idx[index++] = (unsigned short)following;
        }
    }
    w.count = (int)index;
    s.colour = c.colour2;
    s.colour2 = c.colour;
    s.tex0 = c.tex_b;
    s.tex1 = c.tex_a;
    int list;
    if (c.flags & 0x400)
        list = 5;
    else
        list = type;
    w.queued = 1; w.type = list;
    if (w.debug)
        w.boxes = 1;
}

static int fails = 0;
static void check(bool ok, const char* what) { if (!ok) { std::printf("FAIL %s\n", what); ++fails; } }
static bool near(float a, float b) { return std::fabs(a - b) < 1e-4f; }

int main()
{
    World w = {};
    w.vp_tlx = 0; w.vp_tly = 0; w.vp_brx = 640; w.vp_bry = 480; w.layer_enabled = true;
    Clock c = {};
    c.tlx = 100; c.tly = 100; c.brx = 200; c.bry = 300; c.colour = 0xff102030; c.colour2 = 0x80405060;
    c.angle_start = 0.1f; c.angle_end = 0.5f; c.interpolate = 0.5f; c.tex_a = 11; c.tex_b = 22;

    // angle 0.3: segment ceil(1.7) = 2, fraction fmod(0.175, 0.25) * 4 = 0.7, split corner 2
    Render(c, 2, w);
    check(w.queued == 1 && w.type == 2, "queued with the given type");
    const unsigned short want[18] = { 0,1,2, 0,2,6, 0,6,3, 0,3,4, 0,4,5, 0,5,1 };
    check(w.count == 18, "index count 18");
    bool same = true;
    for (int k = 0; k < 18; ++k) same = same && w.sub.idx[k] == want[k];
    check(same, "clock index list, split at corner 2");
    check(w.sub.index_offset == 6, "index offset after the first split triangle");
    check(near(w.sub.v[6].X, 200.0f) && near(w.sub.v[6].Y, 100.0f + 0.7f * 200.0f), "seventh vertex 70% from TR to BR");
    check(near(w.sub.v[6].U, 1.0f) && near(w.sub.v[6].V, 0.7f), "seventh vertex UV");
    check(w.sub.colour == 0x80405060 && w.sub.colour2 == 0xff102030 && w.sub.tex0 == 22 && w.sub.tex1 == 11, "colours and textures swapped");
    check(w.boxes == 0, "no box without the debug flag");

    // angle 0.05 (near the top): segment ceil(0.7) = 1, fraction fmod(0.05, 0.125) * 8 = 0.4, split at corner 1
    c.angle_start = 0.05f; c.angle_end = 0.05f; c.flags = 0x400; w.debug = true;
    Render(c, 2, w);
    check(w.type == 5, "displacement buffer when flag 0x400 is set");
    check(w.boxes == 1, "debug box drawn");
    check(w.sub.idx[0] == 0 && w.sub.idx[1] == 1 && w.sub.idx[2] == 6 && w.sub.index_offset == 3 && w.sub.idx[5] == 2, "split at corner 1");
    check(near(w.sub.v[6].X, 150.0f + 0.4f * 50.0f) && near(w.sub.v[6].Y, 100.0f), "seventh vertex 40% along the top edge");

    // angle clamped to 1 (interpolation past the end): fraction 1, segment ceil(4.5) = 5 -> next 1, split at corner 5, last index 1
    c.angle_start = 0.5f; c.angle_end = 1.5f; c.interpolate = 1.0f; c.flags = 0;
    Render(c, 7, w);
    check(w.type == 7, "given type again without flag 0x400");
    check(w.sub.idx[12] == 0 && w.sub.idx[13] == 5 && w.sub.idx[14] == 6 && w.sub.index_offset == 15 && w.sub.idx[17] == 1, "split at corner 5 wrapping to 1");
    check(near(w.sub.v[6].X, 150.0f) && near(w.sub.v[6].Y, 100.0f), "seventh vertex at the next corner when full");

    // angle clamped to 0: fraction 0, segment ceil(0.5) = 1
    c.angle_start = -0.5f; c.angle_end = -0.5f;
    Render(c, 3, w);
    check(w.sub.index_offset == 3 && near(w.sub.v[6].X, 150.0f), "empty clock: split at corner 1, seventh vertex on the top middle");

    // rejections
    World r = w;
    r.vp_brx = 50; Render(c, 3, r); check(r.queued == 0, "off the viewport");
    r = w; r.mask_bank = true; r.mask_outside = true; r.mtlx = 300; r.mtly = 0; r.mbrx = 400; r.mbry = 400;
    Render(c, 3, r); check(r.queued == 0, "outside the alpha mask");
    r.mask_outside = false; Render(c, 3, r); check(r.queued == 1, "alpha mask ignored when objects outside it are not masked");
    r = w; Clock t = c; t.colour = 0x00ffffff; Render(t, 3, r); check(r.queued == 0, "colour alpha 0");
    r = w; r.layer_enabled = false; Render(c, 3, r); check(r.queued == 0, "layer disabled");

    if (fails == 0)
        std::printf("00bafa00_TEST PASS\n");
    return 0;
}