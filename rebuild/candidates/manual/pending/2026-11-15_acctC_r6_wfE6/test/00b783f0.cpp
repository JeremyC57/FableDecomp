// Behaviour test for 0x00B783F0 CEngineWaterRenderer::Render(const CEngineComponentRenderParams&): a host model of the row with
// recording stand-ins for UpdateConstants, CreateDynamicTextures, the render-state setters, the ten passes and the tail calls.
// Checks: all ten render lists empty and no background sea -> nothing at all (not even UpdateConstants); CreateDynamicTextures gets
// (fg || bg, seaRefl || seaNonRefl || seaMid || seaBg, iceFg || iceBg) and a false result stops before any state change; a background
// sea alone (HaveBackgroundSea && EnableSea) is enough to render; fog off / common states / fog on / ResetWorldTransform before the
// passes; the bump matrix is (f, 0, 0, f) with EnableBumpMap else all zeros; foreground / ice / sea-foreground passes need
// HighendWaterEnabled, the background and sea mid / background passes do not; the sea block (restore, four detaches, env map + mipmaps
// only when high-end, common states twice with fog off / on) runs only with EnableSea and a non-reflective / mid / background sea;
// the sea background pass is preceded by alpha test off + alpha blend off; the tail always selects null buffers and finishes the
// dynamic textures.
#include <cstdio>
#include <cstring>
enum { UPDATE, CREATE, FOG, BUMP, ATEST, BLEND, RESET, RF, RB, RIF, RIB, RSR, RESTORE, ATTACH, ENVMAP, MIPS, RSNR, RSM, RSB, SELV, SELI, FINISH };
struct Ev { int k; int a, b, c; float f; };
struct Log { Ev e[128]; int n; };
static void Put(Log& g, int k, int a = 0, int b = 0, int c = 0, float f = 0) { Ev& e = g.e[g.n++]; e.k = k; e.a = a; e.b = b; e.c = c; e.f = f; }
struct World {
    int lists[10];   // Fg, Bg, AlphaedBg, IceFg, IceBg, IceAlphaedBg, SeaRefl, SeaNonRefl, SeaMid, SeaAlphaedMid (element counts)
    bool haveBgSea, enableSea, enableFg, enableBg, enableIceFg, enableIceBg, enableSeaFg, enableSeaMid, enableSeaBg, bump, highend;
    float factor; bool createOk;
};
static void Common(Log& g, const World& w)
{
    float f = w.bump ? w.factor : 0.0f;
    Put(g, BUMP, 1, 0, 0, f); Put(g, BUMP, 2, 0, 0, f);
    Put(g, ATEST, 1); Put(g, BLEND, 1);
}
static void Row(Log& g, const World& w)
{
    bool fg = w.lists[0] != 0, bg = w.lists[1] != 0 || w.lists[2] != 0;
    bool iceFg = w.lists[3] != 0, iceBg = w.lists[4] != 0 || w.lists[5] != 0;
    bool seaRefl = w.lists[6] != 0, seaNonRefl = w.lists[7] != 0, seaMid = w.lists[8] != 0 || w.lists[9] != 0;
    bool seaBg = w.haveBgSea && w.enableSea;
    if (!(iceFg || iceBg) && !fg && !bg && !seaRefl && !seaNonRefl && !seaMid && !seaBg) return;
    Put(g, UPDATE);
    Put(g, CREATE, fg || bg, seaRefl || seaNonRefl || seaMid || seaBg, iceFg || iceBg);
    if (!w.createOk) return;
    Put(g, FOG, 0); Common(g, w); Put(g, FOG, 1); Put(g, RESET);
    if (w.enableFg && fg && w.highend) Put(g, RF);
    if (w.enableBg && bg) Put(g, RB);
    if (w.enableIceFg && iceFg && w.highend) Put(g, RIF);
    if (w.enableIceBg && iceBg && w.highend) Put(g, RIB);
    if (w.enableSea && w.enableSeaFg && seaRefl && w.highend) Put(g, RSR);
    if (w.enableSea && (seaNonRefl || seaMid || seaBg)) {
        Put(g, RESTORE);
        for (int s = 0; s < 4; ++s) Put(g, ATTACH, s);
        if (w.highend) { Put(g, ENVMAP); Put(g, MIPS); }
        Put(g, FOG, 0); Common(g, w); Put(g, FOG, 1); Common(g, w);
        if (w.enableSeaFg && seaNonRefl && w.highend) Put(g, RSNR);
        if (w.enableSeaMid && seaMid) Put(g, RSM);
        if (w.enableSeaBg && seaBg) { Put(g, ATEST, 0); Put(g, BLEND, 0); Put(g, RSB); }
    }
    Put(g, SELV); Put(g, SELI); Put(g, FINISH);
}
static int bad = 0;
static void Expect(bool c, const char* what) { if (!c) { printf("mismatch: %s\n", what); bad = 1; } }
static int Find(const Log& g, int k, int from = 0) { for (int i = from; i < g.n; ++i) if (g.e[i].k == k) return i; return -1; }
static int Count(const Log& g, int k) { int n = 0; for (int i = 0; i < g.n; ++i) if (g.e[i].k == k) ++n; return n; }
int main()
{
    World all = { { 1, 1, 0, 1, 0, 1, 1, 1, 0, 1 }, true, true, true, true, true, true, true, true, true, true, true, 0.75f, true };
    World none; memset(&none, 0, sizeof none); none.enableSea = true; none.createOk = true;
    Log g;
    g.n = 0; Row(g, none); Expect(g.n == 0, "all empty renders nothing");
    World w = none; w.haveBgSea = true; w.enableSea = false; g.n = 0; Row(g, w); Expect(g.n == 0, "bg sea needs EnableSea");
    w.enableSea = true; w.enableSeaBg = true; g.n = 0; Row(g, w);
    Expect(g.n > 0 && g.e[1].k == CREATE && g.e[1].a == 0 && g.e[1].b == 1 && g.e[1].c == 0, "bg sea alone -> create(0,1,0)");
    Expect(Find(g, RSB) > Find(g, BLEND, Find(g, RESTORE)), "sea background after alpha off");
    w = none; w.lists[5] = 3; g.n = 0; Row(g, w);
    Expect(g.e[1].a == 0 && g.e[1].b == 0 && g.e[1].c == 1, "ice alphaed bg -> create(0,0,1)");
    w = none; w.lists[2] = 1; g.n = 0; Row(g, w); Expect(g.e[1].a == 1 && g.e[1].c == 0, "alphaed bg -> create(1,0,0)");
    w = all; w.createOk = false; g.n = 0; Row(g, w); Expect(g.n == 2, "failed create stops");
    g.n = 0; Row(g, all);
    Expect(g.e[0].k == UPDATE && g.e[2].k == FOG && g.e[2].a == 0 && g.e[3].k == BUMP && g.e[3].f == 0.75f, "set-up A + bump factor");
    Expect(Count(g, BUMP) == 6 && Count(g, RESTORE) == 1 && Count(g, ATTACH) == 4 && Count(g, MIPS) == 1, "sea block counts");
    Expect(Find(g, RF) < Find(g, RB) && Find(g, RB) < Find(g, RIF) && Find(g, RIF) < Find(g, RIB) && Find(g, RIB) < Find(g, RSR)
           && Find(g, RSR) < Find(g, RESTORE) && Find(g, RSNR) < Find(g, RSM) && Find(g, RSM) < Find(g, RSB), "pass order");
    Expect(g.e[g.n - 3].k == SELV && g.e[g.n - 1].k == FINISH, "tail");
    w = all; w.bump = false; g.n = 0; Row(g, w); Expect(g.e[3].k == BUMP && g.e[3].f == 0.0f, "no bump map -> zeros");
    w = all; w.highend = false; g.n = 0; Row(g, w);
    Expect(Find(g, RF) < 0 && Find(g, RIF) < 0 && Find(g, RIB) < 0 && Find(g, RSR) < 0 && Find(g, RSNR) < 0 && Find(g, ENVMAP) < 0
           && Find(g, RB) > 0 && Find(g, RSM) > 0 && Find(g, RSB) > 0, "high-end gating");
    w = all; w.enableSea = false; g.n = 0; Row(g, w);
    Expect(Find(g, RESTORE) < 0 && Find(g, RSR) < 0 && g.e[1].b == 1, "EnableSea off skips sea passes (seaBg false)");
    w = all; w.lists[7] = 0; w.lists[8] = 0; w.lists[9] = 0; w.haveBgSea = false; g.n = 0; Row(g, w);
    Expect(Find(g, RSR) > 0 && Find(g, RESTORE) < 0, "reflective sea alone: no sea block");
    printf(bad ? "00b783f0_TEST FAIL\n" : "00b783f0_TEST PASS\n");
    return bad;
}
