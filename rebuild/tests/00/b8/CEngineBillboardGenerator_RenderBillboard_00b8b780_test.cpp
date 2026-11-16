// Behaviour test for 0x00B8B780 CEngineBillboardGenerator::RenderBillboard: a host model of the row's decisions with recording
// stand-ins for the engine. Checked: the depth surface choice (the billboard's own Z buffer, else the engine scratch D24S8 sized by
// the texture's level-0 width), the sphere-fitted transform (translate by -centre, scale 1/radius, camera rotation without its
// translation, z scaled 0.5 and biased +0.5) and the billboard outputs derived from it (Scale = 2 * radius, Offset = translation *
// radius with X negated and Z * -2), the pre-loop states (alpha test on, blending off, alpha ref = the argument, test func GREATER,
// fog off), per primitive: graphic 0 skipped, the mip level floor(width * stretch * screenSize / (distance * multiplier)) clamped to
// 1..255 and reduced to its top bit, a failed block set-up skipped (no vertex buffer, no draw), then the flush; per lower mip level:
// the screen quad (0 / size corners, u / v 0..1 plus half a texel, rhw 1, z 0), indices 0 1 2 1 3 2, z buffer off, select-arg1 on
// stage 0, blending and alpha test off, LOD bias -1, one indexed draw of 2 triangles from 4 vertices.
#include <cstdio>
#include <cmath>
#include <cstring>

static unsigned long TopBit(unsigned char v) { unsigned long b = 0; while (v >>= 1) ++b; return b; }
static long FloorToLong(float f) { return (long)floor(f); }

struct SPrimitive { unsigned long graphic; long width; float stretch; bool setup_ok; };
struct SLog {
    bool own_zbuffer; unsigned long scratch_width;
    bool alpha_test, alpha_blend, fog; long alpha_ref; int test_func;          // 5 = GREATER in the engine's ALPHA_TEST_FUNC order
    long top_bits[8]; long top_bit_count; long draws; long selects; bool flushed;
    long quad_levels[8]; long quad_count; float quad_x1[8], quad_u0[8], quad_v1[8];
    float scale, off_x, off_y, off_z;
};

// The model (the same decisions as the row, written over plain values).
static void Model(SLog& log, bool has_zbuffer, long width0, const float centre[3], float radius, const SPrimitive* prims, long count,
                  float screenSize, float distance, float multiplier, long alphaRef, long levels, const long* level_widths)
{
    memset(&log, 0, sizeof(log));
    log.own_zbuffer = has_zbuffer;
    if (!has_zbuffer) log.scratch_width = (unsigned long)width0;
    // world = T(-c) * S(1/r) * R(view, translation cleared; identity here) * S(1, 1, 0.5), then E43 += 0.5
    float scale = 1.0f / radius;
    float tx = -centre[0] * scale, ty = -centre[1] * scale, tz = -centre[2] * scale * 0.5f + 0.5f;
    log.scale = radius * 2.0f;
    log.off_x = -(tx * radius); log.off_y = ty * radius; log.off_z = tz * radius * -2.0f;
    log.alpha_test = true; log.alpha_blend = false; log.alpha_ref = alphaRef; log.test_func = 5; log.fog = false;
    for (long i = 0; i < count; ++i) {
        const SPrimitive& p = prims[i];
        if (p.graphic == 0) continue;
        float mip = (float)p.width * p.stretch * screenSize / (distance * multiplier);
        long level = FloorToLong(mip);
        if (level < 1) level = 1; else if (level > 255) level = 255;
        log.top_bits[log.top_bit_count++] = (long)TopBit((unsigned char)level);
        if (!p.setup_ok) continue;
        ++log.selects; ++log.draws;
    }
    log.flushed = true;
    for (long level = 1; level < levels; ++level) {
        float size = (float)level_widths[level];
        float half = 0.5f / size;
        log.quad_levels[log.quad_count] = level;
        log.quad_x1[log.quad_count] = size; log.quad_u0[log.quad_count] = 0.0f + half; log.quad_v1[log.quad_count] = 1.0f + half;
        ++log.quad_count; ++log.draws;
    }
}

static int Near(float a, float b) { return fabs(a - b) < 1e-4f; }

int main()
{
    int ok = 1;
    const float centre[3] = { 2.0f, -4.0f, 6.0f };
    SPrimitive prims[5] = {
        { 0, 64, 1.0f, true },        // graphic 0: skipped, no mip level
        { 7, 64, 1.0f, true },        // 64 * 1 * 100 / (10 * 2) = 320 -> clamp 255 -> top bit 7
        { 9, 16, 0.5f, false },       // 16 * 0.5 * 100 / 20 = 40 -> top bit 5; set-up fails: no draw
        { 3, 1, 0.01f, true },        // 0.05 -> floor 0 -> clamp 1 -> top bit 0
        { 4, 32, 0.375f, true },      // 32 * 0.375 * 100 / 20 = 60 -> top bit 5
    };
    const long widths[4] = { 64, 32, 16, 8 };
    SLog log;
    Model(log, false, 64, centre, 4.0f, prims, 5, 100.0f, 10.0f, 2.0f, 0x40, 4, widths);
    ok &= !log.own_zbuffer && log.scratch_width == 64;
    ok &= Near(log.scale, 8.0f) && Near(log.off_x, 2.0f) && Near(log.off_y, 4.0f) && Near(log.off_z, 2.0f);   // off_z = c.z - r
    ok &= log.alpha_test && !log.alpha_blend && log.alpha_ref == 0x40 && log.test_func == 5 && !log.fog;
    ok &= log.top_bit_count == 4 && log.top_bits[0] == 7 && log.top_bits[1] == 5 && log.top_bits[2] == 0 && log.top_bits[3] == 5;
    ok &= log.selects == 3 && log.flushed;
    ok &= log.quad_count == 3 && log.quad_levels[0] == 1 && log.quad_levels[2] == 3;
    ok &= Near(log.quad_x1[0], 32.0f) && Near(log.quad_u0[0], 0.5f / 32.0f) && Near(log.quad_v1[2], 1.0f + 0.5f / 8.0f);
    ok &= log.draws == 3 + 3;
    Model(log, true, 64, centre, 4.0f, prims, 0, 100.0f, 10.0f, 2.0f, 1, 1, widths);   // own Z buffer, no primitives, one level
    ok &= log.own_zbuffer && log.scratch_width == 0 && log.quad_count == 0 && log.draws == 0 && log.alpha_ref == 1;
    printf(ok ? "00b8b780_TEST PASS\n" : "00b8b780_TEST FAIL\n");
    return 0;
}