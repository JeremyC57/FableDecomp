// Behaviour test for 0x00B89C30 CEngineScreenEffectOutlineGlow::ApplyOutline: a host model of the row's decisions with recording
// stand-ins for the engine. Checked: the bail-out (no outline texture -> no capture block, no state, no draw), the saved render
// target / viewport, the blur set-up states (blending, alpha test, Z, dither, stencil off, no culling, point filtering and clamp
// addressing on stages 0..3), pass 0 (OutlineToBlur shaders, constants c0 = 0, c1 = (1/256, 0), c2 = (0, 1/256), c3 = (1/256, 1/256),
// Blur1 as target, the outline texture on 4 stages), BlurStepCount ping-pong passes (c1 = (-u, -v), c2 = (u, -v), c3 = (0, v) into
// Blur2 from Blur1, then c1 = (-u, v), c2 = (u, v), c3 = (0, -v) into Blur1 from Blur2), the back-screen restore, camera refresh and
// release of the saved target, and the composite states (linear filtering on stage 0, src-alpha / inv-src-alpha blending, alpha test
// >= alpha ref 1, Z off, no culling, dither on, stencil equal 4 with mask 4, pass -> zero) with one draw of Blur1 on 1 stage.
#include <cstdio>
#include <cmath>
#include <cstring>

enum { TEX_NONE = 0, TEX_OUTLINE = 1, TEX_BLUR1 = 2, TEX_BLUR2 = 3 };
enum { TARGET_NONE = 0, TARGET_BLUR1 = 2, TARGET_BLUR2 = 3, TARGET_BACK = 9 };
struct SConst { float c[4][4]; };
struct SDraw { int target; int texture; int stages; int verts; SConst consts; };
struct SLog {
    bool captured, saved, released, camera_refreshed;
    bool blend, alpha_test, zbuffer, dither, stencil; int cull;          // cull 0 = none
    int filter[4]; int uv[4];                                           // filter 0 point / 1 linear, uv 0 clamp
    int alpha_func; long alpha_ref; int src_blend, dst_blend; int stencil_func, stencil_pass; long stencil_mask, stencil_ref;
    int target; SConst consts; SDraw draws[32]; int draw_count;
};

static void SetConst(SLog& log, int i, float x, float y) { log.consts.c[i][0] = x; log.consts.c[i][1] = y; log.consts.c[i][2] = 0; log.consts.c[i][3] = 0; }
static void Draw(SLog& log, int texture, int stages, int verts)
{
    SDraw& d = log.draws[log.draw_count++];
    d.target = log.target; d.texture = texture; d.stages = stages; d.verts = verts; d.consts = log.consts;
}

// The model: the same decisions as the row, written over plain values.
static void Model(SLog& log, bool has_outline, long steps, float u, float v)
{
    memset(&log, 0, sizeof(log));
    log.target = TARGET_NONE; log.blend = log.alpha_test = log.zbuffer = log.stencil = true; log.cull = 2; log.dither = true;
    if (!has_outline) return;
    log.captured = true; log.saved = true;
    log.blend = false; log.alpha_test = false; log.zbuffer = false; log.cull = 0; log.dither = false; log.stencil = false;
    for (int s = 0; s < 4; ++s) { log.filter[s] = 0; log.uv[s] = 0; }
    SetConst(log, 0, 0.0f, 0.0f); SetConst(log, 1, 1.0f / 256.0f, 0.0f); SetConst(log, 2, 0.0f, 1.0f / 256.0f); SetConst(log, 3, 1.0f / 256.0f, 1.0f / 256.0f);
    log.target = TARGET_BLUR1; Draw(log, TEX_OUTLINE, 4, 1);
    for (long i = 0; i < steps; ++i) {
        SetConst(log, 0, 0.0f, 0.0f); SetConst(log, 1, -u, -v); SetConst(log, 2, u, -v); SetConst(log, 3, 0.0f, v);
        log.target = TARGET_BLUR2; Draw(log, TEX_BLUR1, 4, 2);
        SetConst(log, 0, 0.0f, 0.0f); SetConst(log, 1, -u, v); SetConst(log, 2, u, v); SetConst(log, 3, 0.0f, -v);
        log.target = TARGET_BLUR1; Draw(log, TEX_BLUR2, 4, 2);
    }
    log.target = TARGET_BACK; log.camera_refreshed = true; log.released = true;
    log.filter[0] = 1;
    log.blend = true; log.alpha_test = true; log.alpha_func = 7; log.alpha_ref = 1; log.src_blend = 5; log.dst_blend = 6;
    log.zbuffer = false; log.cull = 0; log.dither = true; log.stencil = true; log.stencil_func = 3; log.stencil_pass = 2;
    log.stencil_mask = 4; log.stencil_ref = 4;
    Draw(log, TEX_BLUR1, 1, 3);
}

static int Near(float a, float b) { return fabs(a - b) < 1e-6f; }

int main()
{
    int ok = 1;
    SLog log;
    Model(log, false, 3, 1.0f / 128.0f, 1.0f / 128.0f);                 // no outline texture: untouched
    ok &= !log.captured && !log.saved && log.draw_count == 0 && log.blend && log.stencil && log.target == TARGET_NONE;
    const float u = 1.0f / 128.0f, v = 1.0f / 64.0f;
    Model(log, true, 2, u, v);
    ok &= log.captured && log.saved && log.released && log.camera_refreshed && log.target == TARGET_BACK;
    ok &= log.draw_count == 1 + 2 * 2 + 1;
    ok &= log.draws[0].target == TARGET_BLUR1 && log.draws[0].texture == TEX_OUTLINE && log.draws[0].stages == 4;
    ok &= Near(log.draws[0].consts.c[1][0], 1.0f / 256.0f) && Near(log.draws[0].consts.c[2][1], 1.0f / 256.0f) && Near(log.draws[0].consts.c[3][0], 1.0f / 256.0f);
    ok &= log.draws[1].target == TARGET_BLUR2 && log.draws[1].texture == TEX_BLUR1;
    ok &= Near(log.draws[1].consts.c[1][0], -u) && Near(log.draws[1].consts.c[1][1], -v) && Near(log.draws[1].consts.c[2][0], u) && Near(log.draws[1].consts.c[3][1], v);
    ok &= log.draws[2].target == TARGET_BLUR1 && log.draws[2].texture == TEX_BLUR2;
    ok &= Near(log.draws[2].consts.c[1][1], v) && Near(log.draws[2].consts.c[2][1], v) && Near(log.draws[2].consts.c[3][1], -v) && Near(log.draws[2].consts.c[3][0], 0.0f);
    ok &= log.draws[4].target == TARGET_BLUR1 && log.draws[4].texture == TEX_BLUR2;
    ok &= log.draws[5].target == TARGET_BACK && log.draws[5].texture == TEX_BLUR1 && log.draws[5].stages == 1 && log.draws[5].verts == 3;
    ok &= log.filter[0] == 1 && log.filter[1] == 0 && log.filter[3] == 0 && log.uv[2] == 0;
    ok &= log.blend && log.alpha_test && log.alpha_func == 7 && log.alpha_ref == 1 && log.src_blend == 5 && log.dst_blend == 6;
    ok &= !log.zbuffer && log.cull == 0 && log.dither && log.stencil && log.stencil_func == 3 && log.stencil_pass == 2;
    ok &= log.stencil_mask == 4 && log.stencil_ref == 4;
    Model(log, true, 0, u, v);                                           // no blur steps: pass 0 and the composite only
    ok &= log.draw_count == 2 && log.draws[1].texture == TEX_BLUR1 && log.draws[1].target == TARGET_BACK;
    printf(ok ? "00b89c30_TEST PASS\n" : "00b89c30_TEST FAIL\n");
    return 0;
}