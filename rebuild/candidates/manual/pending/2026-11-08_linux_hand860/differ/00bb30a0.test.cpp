// Behaviour test for 0x00BB30A0 CEngineClothRenderer::Render2DClothPrimitive: pass 1 sets z-write on, blend off, alpha test with the
// primitive's alpha reference and no colour writes; other passes test alpha against (ref * alpha) >> 8 when flag 2 is set (else off,
// ref 0) and blend source-alpha / inverse-source-alpha with z-write off when flag 1 is set (else z-write on, blend off). Point
// filtering on stage 1 unless the 2D cloth settings ask for linear filtering; stage 1 clamps U and V. The vertex shader is the spot
// shadow one (flags 0x200 | 0x10), the shadow one (0x200) or the plain tri shader; the tint colour (B, G, R bytes) and the sub-mesh
// alpha go out as pixel-shader constant 2 when pixel shaders are on, else as the packed texture factor. A preloading texture skips the
// lighting update and the draw; the device streams and texture stages are released either way.
#include <cstdio>
#include <cmath>

struct State { bool zwrite, blend, atest, cwrite; unsigned long aref; int src, dst; int mag, min, mip; int u, v; };
struct Result { State s; int shader; float c[4]; unsigned long tf; bool drew; int flushes; long selfillum; bool fogoff; };

static Result Render(int pass, unsigned long flags, unsigned char alpha, unsigned char aref, bool linear, bool ps,
                     unsigned char r, unsigned char g, unsigned char b, bool preloading, long selfillum)
{
    Result res = {};
    res.s.zwrite = true; res.s.cwrite = true; res.s.mag = res.s.min = 2; res.s.mip = 0; res.s.u = res.s.v = 0;
    if (pass == 1) {
        res.s.zwrite = true; res.s.blend = false; res.s.atest = true; res.s.aref = aref; res.s.cwrite = false;
    } else {
        if (flags & 2) { res.s.atest = true; res.s.aref = (aref * alpha) >> 8; }
        else { res.s.atest = false; res.s.aref = 0; }
        if (flags & 1) { res.s.zwrite = false; res.s.blend = true; res.s.src = 5; res.s.dst = 6; res.s.atest = true; }
        else { res.s.zwrite = true; res.s.blend = false; }
    }
    if (!linear) { res.s.mag = 1; res.s.min = 1; res.s.mip = 2; }
    res.s.u = 3; res.s.v = 3;
    res.shader = (flags & 0x200) ? ((flags & 0x10) ? 2 : 1) : 0;
    float k = 1.0f / 255.0f;
    float c[4] = { r * k, g * k, b * k, alpha * k };
    if (ps) for (int i = 0; i < 4; ++i) res.c[i] = c[i];
    else res.tf = ((unsigned long)(unsigned char)(long)(c[3] * 255.0f) << 24) | ((unsigned long)(unsigned char)(long)(c[0] * 255.0f) << 16)
                | ((unsigned long)(unsigned char)(long)(c[1] * 255.0f) << 8) | (unsigned long)(unsigned char)(long)(c[2] * 255.0f);
    if (!preloading) { res.selfillum = selfillum; res.fogoff = true; res.drew = true; }
    ++res.flushes;
    return res;
}

int main()
{
    bool ok = true;
    Result a = Render(1, 3, 128, 200, false, false, 10, 20, 30, false, 40);
    ok = ok && a.s.zwrite && !a.s.blend && a.s.atest && a.s.aref == 200 && !a.s.cwrite && a.s.mag == 1 && a.s.mip == 2 && a.drew;
    Result b = Render(0, 2 | 1, 128, 200, true, false, 255, 0, 0, false, 40);
    ok = ok && !b.s.zwrite && b.s.blend && b.s.atest && b.s.aref == 100 && b.s.src == 5 && b.s.mag == 2 && b.s.u == 3
        && b.tf == 0x80FF0000ul && b.shader == 0 && b.selfillum == 40 && b.fogoff;
    Result c = Render(0, 0x210, 51, 9, true, true, 0, 255, 0, true, 7);
    ok = ok && !c.s.atest && c.s.aref == 0 && c.s.zwrite && c.shader == 2 && c.c[1] == 1.0f && std::fabs(c.c[3] - 0.2f) < 1e-6f
        && !c.drew && c.flushes == 1 && c.selfillum == 0;
    Result d = Render(0, 0x200, 255, 9, true, true, 0, 0, 0, false, 7);
    ok = ok && d.shader == 1 && d.drew && d.flushes == 1;
    std::printf(ok ? "00bb30a0_TEST PASS\n" : "00bb30a0_TEST FAIL\n");
    return ok ? 0 : 1;
}
