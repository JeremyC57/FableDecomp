// Behaviour test for 0x00BCA950 CEnginePrimitiveManagerSpriteTrails::RenderSpriteTrail.
// A copy of the row's logic over plain models (the vertex buffer records what the row writes; the world matrix is a scale +
// translation, the camera view transforms are identities), checked against independently written expectations: the two early
// exits (fewer than two points after the skip, length below DELTA_NOTIONAL_ZERO incl. MaxLength 2), the vertex count 16 * (points - 1)
// + 2 handed to the buffer and the strip of count - 2 triangles, the first segment's step 0 and the later segments' steps 1..8 (every
// vertex written exactly once), the Hermite curve through the trail points in reverse order from NextPos, the camera-facing side
// vectors scaled to Width, the U pair 0.99 / 0.01, V from the arc fraction, the head / mid / tail colour blend with doubled alpha,
// the premultiplied colours, and the skip fraction's clamps (segment 0 starts at t >= fraction, segment MaxLength - 2 stops at t <= fraction).
#include <cstdio>
#include <cmath>

struct Point { float x, y, z; };
struct Trail {
    Point p[16]; long size; float skip; long next; long max_length; float width;
    unsigned char head[4], mid[4], tail[4];         // R G B A
    float scale; Point offset;                      // world matrix: p * scale + offset
};
struct Vertex { float x, y, z; unsigned char r, g, b, a; float u, v; };
struct Out { long buffer_count; long triangles; long written; Vertex vtx[256]; };

static Point Sub(Point a, Point b) { Point r = { a.x - b.x, a.y - b.y, a.z - b.z }; return r; }
static Point Add(Point a, Point b) { Point r = { a.x + b.x, a.y + b.y, a.z + b.z }; return r; }
static Point Mul(Point a, float s) { Point r = { a.x * s, a.y * s, a.z * s }; return r; }
static long Near(float f) { return (long)std::floor(f + 0.5f); }

static bool RenderSpriteTrail(const Trail& trail, bool premultiplied, Out& o)
{
    o.buffer_count = o.triangles = o.written = 0;
    long trail_size = trail.size;
    long skip = (long)std::floor(trail.skip);
    if (skip < 0) skip = 0;
    long points = trail_size - skip;
    if (points < 2) return false;
    long start = trail.next % trail_size;
    float fraction = trail.skip - skip;
    if (fraction < 0.0f) fraction = 0.0f;
    long segments = points - 1;
    float a = (float)(trail.max_length - 2), b = segments - fraction;
    float length = b < a ? b : a;
    if (length < 1e-4f) return false;
    unsigned long vertex_count = (points - 1) * 16 + 2;
    Point tangents[16] = {};                         // the row's CArray(points); entries it never sets read as zero here
    for (long i = 0; i < points; ++i) {
        long n = points - i;
        long next = trail_size - 1 < n ? trail_size - 1 : n;
        long previous = n - 2 > 0 ? n - 2 : 0;
        float scale = next == previous + 2 ? 0.5f : 1.0f;
        long index = (n + start - 1) % trail_size;
        tangents[index] = Mul(Mul(Sub(trail.p[(previous + start) % trail_size], trail.p[(next + start) % trail_size]), scale), trail.scale);
    }
    o.buffer_count = (long)vertex_count;
    float colours[3][4];
    const unsigned char* src[3] = { trail.head, trail.mid, trail.tail };
    for (int c = 0; c < 3; ++c) {
        for (int k = 0; k < 4; ++k) colours[c][k] = src[c][k] * (1.0f / 255.0f);
        colours[c][3] *= 2.0f;
        if (premultiplied) for (int k = 0; k < 3; ++k) colours[c][k] *= colours[c][3];
    }
    float one_over_length = 1.0f / length;
    long first_step = 0, current = start - 1, previous = current - 1;
    for (long segment = 0; segment < segments; ++segment) {
        if (current < 0) current += trail_size;
        if (previous < 0) previous += trail_size;
        for (long step = first_step; step <= 8; ++step) {
            float t = step * 0.125f;
            if (segment == 0) { if (t < fraction) t = fraction; }
            else if (segment == trail.max_length - 2) { if (t > fraction) t = fraction; }
            float t2 = t * t, t3 = t2 * t;
            float h1 = 2.0f * t3 - 3.0f * t2 + 1.0f, h2 = 3.0f * t2 - 2.0f * t3, h3 = t3 - 2.0f * t2 + t, h4 = t3 - t2;
            Point pos = Add(Add(Mul(trail.p[current], h1), Mul(trail.p[previous], h2)), Add(Mul(tangents[current], h3), Mul(tangents[previous], h4)));
            pos = Add(Mul(pos, trail.scale), trail.offset);
            float v = (segment + t - fraction) * one_over_length;
            Point dir = Add(tangents[current], Mul(Sub(tangents[previous], tangents[current]), t));
            Point side = { dir.y, -dir.x, 0.0f };
            float mag = std::sqrt(side.x * side.x + side.y * side.y + side.z * side.z);
            if (mag > 1e-4f) side = Mul(side, trail.width / mag);
            long ci = v < 0.5f ? 0 : 1;
            float blend = v * 2.0f - ci;
            long rgba[4];
            for (int k = 0; k < 4; ++k) rgba[k] = Near((colours[ci][k] * (1.0f - blend) + colours[ci + 1][k] * blend) * 255.0f);
            for (int side_index = 0; side_index < 2; ++side_index) {
                Vertex& w = o.vtx[o.written++];
                Point q = side_index == 0 ? Sub(pos, side) : Add(pos, side);
                w.x = q.x; w.y = q.y; w.z = q.z;
                w.r = (unsigned char)rgba[0]; w.g = (unsigned char)rgba[1]; w.b = (unsigned char)rgba[2]; w.a = (unsigned char)rgba[3];
                w.u = side_index == 0 ? 0.99f : 0.01f; w.v = v;
            }
        }
        --current; --previous; first_step = 1;
    }
    o.triangles = (long)vertex_count - 2;
    return true;
}

static int fails;
static void Check(bool c, const char* what) { if (!c) { ++fails; std::printf("FAIL %s\n", what); } }
static bool Close(float a, float b) { return std::fabs(a - b) < 1e-3f; }

int main()
{
    Trail tr = {};
    for (int k = 0; k < 5; ++k) { tr.p[k].x = (float)k; tr.p[k].y = 0.0f; tr.p[k].z = 0.0f; }
    tr.size = 5; tr.skip = 0.0f; tr.next = 0; tr.max_length = 10; tr.width = 0.5f;
    unsigned char head[4] = { 255, 0, 0, 100 }, mid[4] = { 0, 255, 0, 50 }, tail[4] = { 0, 0, 255, 20 };
    for (int k = 0; k < 4; ++k) { tr.head[k] = head[k]; tr.mid[k] = mid[k]; tr.tail[k] = tail[k]; }
    tr.scale = 1.0f; tr.offset.x = tr.offset.y = tr.offset.z = 0.0f;
    static Out o;

    // Straight trail of 5 points: 4 segments, 9 + 3 * 8 steps = 66 vertices, 64 strip triangles.
    Check(RenderSpriteTrail(tr, false, o), "rendered");
    Check(o.buffer_count == 66 && o.written == 66 && o.triangles == 64, "vertex count");
    Check(Close(o.vtx[0].x, 4.0f) && Close(o.vtx[65].x, 0.0f), "runs from NextPos-1 back to point 0");
    Check(Close(std::fabs(o.vtx[0].y), 0.5f) && Close(o.vtx[0].y, -o.vtx[1].y) && o.vtx[0].z == 0.0f, "side vector of Width");
    Check(o.vtx[0].u == 0.99f && o.vtx[1].u == 0.01f && o.vtx[0].v == 0.0f && Close(o.vtx[65].v, 1.0f), "UVs");
    Check(Close(o.vtx[2].x, 3.875f) && Close(o.vtx[16].x, 3.0f) && Close(o.vtx[18].x, 2.875f) && Close(o.vtx[20].x, 2.75f), "step 0 only on segment 0");
    Check(o.vtx[0].r == 255 && o.vtx[0].g == 0 && o.vtx[0].a == 200, "head colour, alpha doubled");
    Check(o.vtx[65].b == 255 && o.vtx[65].r == 0 && o.vtx[65].a == 40, "tail colour");
    Check(o.vtx[32].g == 255 && o.vtx[32].a == 100 && Close(o.vtx[32].v, 0.5f), "mid colour at v 0.5");

    // Premultiplied: RGB scaled by the doubled alpha.
    Check(RenderSpriteTrail(tr, true, o) && o.vtx[0].r == 200 && o.vtx[65].b == 40 && o.vtx[0].a == 200, "premultiplied");

    // World transform: scale 2, offset (1, 2, 3).
    Trail w = tr; w.scale = 2.0f; w.offset.x = 1.0f; w.offset.y = 2.0f; w.offset.z = 3.0f;
    Check(RenderSpriteTrail(w, false, o) && Close(o.vtx[0].x, 9.0f) && Close(o.vtx[0].z, 3.0f) && Close(o.vtx[0].y + o.vtx[1].y, 4.0f), "world transform");

    // NextPos rotates the start.
    Trail r = tr; r.next = 7;
    Check(RenderSpriteTrail(r, false, o) && Close(o.vtx[0].x, 1.0f), "start from NextPos % size");

    // Early exits.
    Trail e = tr; e.size = 1; e.next = 0; Check(!RenderSpriteTrail(e, false, o) && o.written == 0, "one point");
    e = tr; e.skip = 4.5f; Check(!RenderSpriteTrail(e, false, o), "skip leaves one point");
    e = tr; e.max_length = 2; Check(!RenderSpriteTrail(e, false, o), "MaxLength 2: zero length");
    e = tr; e.skip = -3.0f; Check(RenderSpriteTrail(e, false, o) && o.buffer_count == 66 && o.vtx[0].v == 0.0f, "negative skip clamps to 0");

    // Skip fraction: skip 1.5 -> 4 points, 3 segments, fraction 0.5; MaxLength 4 -> length 2, segment 2 stops at t 0.5.
    Trail f = tr; f.skip = 1.5f; f.max_length = 4;
    Check(RenderSpriteTrail(f, false, o) && o.buffer_count == 50 && o.written == 50, "fraction count");
    Check(Close(o.vtx[0].x, 3.625f) && o.vtx[0].v == 0.0f && Close(o.vtx[8].x, 3.625f) && Close(o.vtx[16].x, 3.0f), "segment 0 starts at t = fraction");
    Check(Close(o.vtx[32].x, 2.0f) && Close(o.vtx[34].x, 1.875f) && Close(o.vtx[38].x, 1.625f) && Close(o.vtx[40].x, 1.5f) && Close(o.vtx[49].x, 1.5f) && Close(o.vtx[49].v, 1.0f), "segment 2 clamps t <= fraction");

    if (!fails) std::printf("00bca950_TEST PASS\n");
    return fails;
}