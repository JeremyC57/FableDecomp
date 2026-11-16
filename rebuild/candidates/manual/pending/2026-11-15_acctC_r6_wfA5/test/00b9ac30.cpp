// Behaviour test for 0x00B9AC30 CEnginePrimitiveManagerRepeatedStaticMeshes::RenderSubPrimitive: with sub-sections the visible
// clip list decides the objects (no visible section = nothing drawn; all objects visible = the primitive's own arrays; otherwise
// the visible runs are compacted into the SIMD block, wind delays only when the primitive has them); a frame without a loaded
// texture draws nothing; the stipple flag (8) picks the stipple shaders on stage 2 (shadowed, 0x200) or 1, uploads constant 3
// from the camera window and sets point / point / wrap / wrap on that stage and attaches the stipple texture; otherwise the
// shadowed or plain shaders; the faded shadow factor is !(flags & 0x200); wind sheer only with wind delays; cull none for a
// two-sided material; alpha test on with the primitive's ref; occlusion test only for a valid handle; shadow-func states for
// shadowed primitives; objects drawn in batches of the constant-layout size (last batch shorter), then flush + restore.
#include <cstdio>
#include <string>
#include <vector>
struct Clip { long start, size; };
struct Prim { long object_count; bool has_sections, has_wind, two_sided, texture_loaded; long alpha_ref; std::vector<Clip> visible; };
struct Out {
    std::string log; std::vector<long> drawn; long stage; float c[4]; bool faded, compacted, wind_copied; long cull_none;
};
static void Render(const Prim& p, unsigned long flags, unsigned long occlusion, long batch, const float win[4], Out& o) {
    long count = p.object_count;
    o.compacted = false; o.wind_copied = false;
    if (p.has_sections) {
        if (p.visible.empty()) return;
        count = 0;
        for (size_t c = 0; c < p.visible.size(); ++c) count += p.visible[c].size;
        if (count != p.object_count) { o.compacted = true; o.wind_copied = p.has_wind; }
    }
    if (!p.texture_loaded) return;
    o.log += "C";                                                    // capture block + specular + layout 4
    if (flags & 8) {
        o.stage = (flags & 0x200) ? 2 : 1;
        o.log += (flags & 0x200) ? "s" : "t";
        float left = win[0], top = win[1], right = win[2], bottom = win[3];
        o.c[0] = (right - left) * 0.015625f; o.c[1] = (bottom - top) * -0.015625f;
        o.c[2] = (float)(left * 0.03125f + 0.015625); o.c[3] = (float)(top * 0.03125f + 0.015625);
        o.log += "PPWWA";
    } else
        o.log += (flags & 0x200) ? "h" : "n";
    o.faded = !(flags & 0x200);
    if (p.has_wind) o.log += "w";
    if (p.two_sided) ++o.cull_none;
    o.log += "a";
    if (occlusion != (unsigned long)-1) o.log += "O";
    if (flags & 0x200) o.log += "S";
    for (long done = 0; ; done += batch) {
        long remaining = count - done;
        long n = remaining >= batch ? batch : remaining;
        if (n <= 0) break;
        o.drawn.push_back(n);
    }
    if (occlusion != (unsigned long)-1) o.log += "E";
    o.log += "F";
}
static int fails = 0;
static void check(bool c, const char* w) { if (!c) { ++fails; std::printf("FAIL %s\n", w); } }
int main() {
    const float win[4] = { 64.0f, 32.0f, 704.0f, 512.0f };
    Prim p; p.object_count = 70; p.has_sections = true; p.has_wind = true; p.two_sided = true; p.texture_loaded = true; p.alpha_ref = 9;
    Out o; o.cull_none = 0;
    Render(p, 8 | 0x200, 5, 32, win, o);
    check(o.log.empty() && o.drawn.empty(), "no visible section: nothing");
    Clip a = { 0, 30 }, b = { 40, 25 }; p.visible.push_back(a); p.visible.push_back(b);
    Render(p, 8 | 0x200, 5, 32, win, o);
    check(o.log == "CsPPWWAwaOSEF" && o.stage == 2 && !o.faded, "stipple shadowed: stage 2, wind, occlusion, shadow states");
    check(o.compacted && o.wind_copied && o.drawn.size() == 2 && o.drawn[0] == 32 && o.drawn[1] == 23, "55 visible: compacted, batches 32 + 23");
    check(o.c[0] == 10.0f && o.c[1] == -7.5f && o.c[2] == 2.015625f && o.c[3] == 1.015625f, "stipple constant 3 from the window");
    check(o.cull_none == 1, "two-sided: cull none");
    o = Out(); o.cull_none = 0; p.has_wind = false; p.two_sided = false; Clip c = { 65, 15 }; p.visible.push_back(c);
    Render(p, 8, (unsigned long)-1, 32, win, o);
    check(o.log == "CtPPWWAaF" && o.stage == 1 && o.faded && !o.compacted, "stipple plain: stage 1, all visible = own arrays, no occlusion");
    check(o.drawn.size() == 3 && o.drawn[2] == 6 && o.cull_none == 0, "70 objects: 32 + 32 + 6");
    o = Out(); o.cull_none = 0; Render(p, 0x200, 1, 70, win, o);
    check(o.log == "ChaOSEF" && o.drawn.size() == 1 && o.drawn[0] == 70 && !o.faded, "shadowed shaders, one batch");
    o = Out(); o.cull_none = 0; Render(p, 0, 1, 64, win, o);
    check(o.log == "CnaOEF" && o.faded && o.drawn.size() == 2, "plain shaders, faded on");
    o = Out(); p.texture_loaded = false; Render(p, 0, 1, 64, win, o);
    check(o.log.empty() && o.drawn.empty(), "texture not loaded: nothing");
    if (!fails) std::printf("00b9ac30_TEST PASS\n");
    return fails;
}
