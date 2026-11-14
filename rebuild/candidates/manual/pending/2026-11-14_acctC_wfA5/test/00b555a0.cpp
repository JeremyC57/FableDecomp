// Behaviour test for 0x00B555A0 CEngineWeatherRenderer::Render: nothing happens while rendering is disabled; otherwise the capture block
// opens first, vertex / pixel shaders are enabled only when off, vertex constant layout 1 is selected, the particle states are set (z write
// off, alpha test on / greater / ref 1, alpha blending on, src-alpha / inv-src-alpha, wrap U / V and clamp W on stages 0..3; an unchanged
// state is not re-queued), the world transform is reset, rain / snow are drawn when active, mist turns z testing off, mirrors all twelve UV
// modes and draws the mist, the index and vertex buffers are unbound, and everything is restored in reverse order (layout, shaders, capture).
#include <cstdio>
#include <cstring>
#include <string>
enum { WRAP = 1, MIRROR = 2, CLAMP = 3, GREATER = 5, SRCA = 5, INVSRCA = 6 };
struct State { unsigned long desired; bool queued; };
struct Rec {
    std::string log; State zwrite, zenable, atest, afunc, aref, ablend, src, dst, u[4], v[4], w[4];
    bool vs, ps; long layout; int queued; bool capture; int restored;
    void Set(State& s, unsigned long v) { if (s.desired == v) return; s.desired = v; if (!s.queued) { s.queued = true; ++queued; } }
};
struct Weather { bool enabled, raining, snowing, mist; };
static void Render(const Weather& w, Rec& r) {
    if (!w.enabled) return;
    r.capture = true; r.log += "C";
    bool old_vs = r.vs, old_ps = r.ps;
    if (old_vs != true) { r.vs = true; r.log += "V"; }
    if (old_ps != true) { r.ps = true; r.log += "P"; }
    long old_layout = r.layout; r.layout = 1; r.log += "L";
    r.Set(r.zwrite, 0); r.Set(r.atest, 1); r.Set(r.afunc, GREATER); r.Set(r.aref, 1); r.Set(r.ablend, 1); r.Set(r.src, SRCA); r.Set(r.dst, INVSRCA);
    for (int s = 0; s < 4; ++s) { r.Set(r.u[s], WRAP); r.Set(r.v[s], WRAP); r.Set(r.w[s], CLAMP); }
    r.log += "T";
    if (w.raining) r.log += "R";
    if (w.snowing) r.log += "S";
    if (w.mist) {
        r.Set(r.zenable, 0);
        for (int s = 0; s < 4; ++s) { r.Set(r.u[s], MIRROR); r.Set(r.v[s], MIRROR); r.Set(r.w[s], MIRROR); }
        r.log += "M";
    }
    r.log += "IB";
    r.layout = old_layout; r.log += "l";
    if (old_vs != r.vs) { r.vs = old_vs; r.log += old_vs ? "V" : "v"; }
    if (old_ps != r.ps) { r.ps = old_ps; r.log += old_ps ? "P" : "p"; }
    r.capture = false; ++r.restored; r.log += "c";
}
static int fails = 0;
static void check(bool c, const char* w) { if (!c) { ++fails; std::printf("FAIL %s\n", w); } }
int main() {
    Rec r; std::memset(&r.zwrite, 0, sizeof(State) * 20); r.vs = false; r.ps = true; r.layout = 7; r.queued = 0; r.capture = false; r.restored = 0;
    Weather w = { false, true, true, true };
    Render(w, r); check(r.log == "" && r.restored == 0, "disabled: nothing");
    w.enabled = true; w.mist = false;
    Render(w, r);
    check(r.log == "CVLTRSIBlvc", "rain + snow order, vertex shaders toggled back off, pixel left alone");
    check(r.layout == 7 && !r.vs && r.ps && r.restored == 1, "layout and shaders restored");
    check(r.zwrite.desired == 0 && r.atest.desired == 1 && r.afunc.desired == GREATER && r.aref.desired == 1 && r.ablend.desired == 1, "particle states");
    check(r.src.desired == SRCA && r.dst.desired == INVSRCA && r.u[3].desired == WRAP && r.w[2].desired == CLAMP, "blend and UV states");
    check(r.queued == 18, "zwrite (already 0) is not queued, the other 18 are");
    r.log = ""; w.raining = false; w.snowing = false; w.mist = true; r.vs = true; r.ps = false;
    Render(w, r);
    check(r.log == "CPLTMIBlpc", "mist only: pixel shaders toggled, mist drawn");
    check(r.u[0].desired == MIRROR && r.v[3].desired == MIRROR && r.w[1].desired == MIRROR && r.zenable.desired == 0, "mist mirrors every stage");
    if (!fails) std::printf("00b555a0_TEST PASS\n");
    return fails;
}
