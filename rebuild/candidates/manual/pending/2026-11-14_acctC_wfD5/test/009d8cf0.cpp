// Behaviour test for 0x009D8CF0 CRenderManager2D::ClearRenderTarget: flags 0 means colour | z | stencil (7); with alternate clears
// off the display manager clears; otherwise fog / specular / blend / alpha test go off, colour writes follow flag 4, flag 1 enables
// z with writes and an always-pass test (else z off), flag 2 enables an always-pass stencil replacing with ref 0 / mask 0xFF (else
// stencil off), and a full-viewport quad (-1..1, z just under 1, rhw 1, the clear colour) is drawn as two indexed triangles inside
// a viewport covering the render target, the old viewport restored afterwards.
#include <cstdio>

struct V { float x, y, z, rhw; unsigned long colour; float u, v; };
struct R {
    bool alternate, fog, spec, blend, atest, cwrite, z, zwrite, stencil;
    int zfunc, sfunc, sop, smask, sref, display_clears, draws; unsigned long display_flags;
    int vp[4]; V quad[4]; unsigned short idx[6];
    void Clear(unsigned long colour, unsigned long flags)
    {
        if (flags == 0) flags = 7;
        if (!alternate) { ++display_clears; display_flags = flags; return; }
        fog = spec = blend = atest = false;
        cwrite = (flags & 4) != 0;
        if (flags & 1) { z = true; zwrite = true; zfunc = 8; } else z = false;
        if (flags & 2) { stencil = true; sfunc = 8; sop = 3; smask = 0xff; sref = 0; } else stencil = false;
        int old[4] = { vp[0], vp[1], vp[2], vp[3] };
        vp[0] = 0; vp[1] = 0; vp[2] = 640; vp[3] = 480;
        const float xs[4] = { -1, 1, 1, -1 }, ys[4] = { 1, 1, -1, -1 };
        for (int i = 0; i < 4; ++i) { V v = { xs[i], ys[i], 0.99999994f, 1.0f, colour, 0, 0 }; quad[i] = v; }
        const unsigned short id[6] = { 0, 1, 2, 0, 2, 3 };
        for (int i = 0; i < 6; ++i) idx[i] = id[i];
        ++draws;
        for (int i = 0; i < 4; ++i) vp[i] = old[i];
    }
};

int main()
{
    R a = R(); a.alternate = false;
    a.Clear(0xff00ff00ul, 0);
    bool ok = a.display_clears == 1 && a.display_flags == 7 && a.draws == 0;
    R b = R(); b.alternate = true; b.fog = true; b.vp[2] = 100;
    b.Clear(0x80112233ul, 4 | 1);
    ok = ok && b.draws == 1 && !b.fog && b.cwrite && b.z && b.zwrite && b.zfunc == 8 && !b.stencil && b.quad[2].y == -1.0f
        && b.quad[1].colour == 0x80112233ul && b.quad[3].rhw == 1.0f && b.idx[5] == 3 && b.vp[2] == 100;
    R c = R(); c.alternate = true;
    c.Clear(0, 2);
    ok = ok && !c.cwrite && !c.z && c.stencil && c.sop == 3 && c.smask == 0xff && c.sref == 0;
    std::printf(ok ? "009d8cf0_TEST PASS\n" : "009d8cf0_TEST FAIL\n");
    return ok ? 0 : 1;
}
