// Behaviour test for 0x007E15C0 CTCGraphicAppearanceMultipleStaticMeshes::OnAppearanceDraw: with no primitive handles yet, one
// persistent static-mesh primitive is added per def entry (mesh index of the entry) and drawing continues; with handles and a
// position-only redraw (ReasonsForDraw == 1) every handle's position is refreshed and nothing is rebuilt; otherwise each handle whose
// def entry has a mesh gets a lighting channel (override: index0 = entry's channel, index1 = blend = 0; else the appearance's channel
// copied whole), is skipped when the debug filter is >= 0 and index0 != 0, and is rebuilt as a static mesh packet: size = render size *
// scale, lit, the appearance's alpha / tint, fade = the entry's override distance or the appearance's fade distance, the parent's
// bullet-time flag, the appearance's mesh effect. Always returns true.
#include <cstdio>
#include <vector>
struct Channel { unsigned char Index0, Index1, Blend, Pad; Channel() : Index0(0), Index1(0), Blend(0), Pad(0) {} };
struct Entry { unsigned long Mesh; bool OverrideRenderFadeDistance; float RenderFadeDistance; bool OverrideLightingChannel; long LightingChannel; };
struct Packet { unsigned long mesh; float size, fade; bool lit, bullet; unsigned char alpha; unsigned long tint; Channel channel; int effect; };
struct Prim { unsigned long mesh; int refreshes; bool hasPacket; Packet packet; };
struct Appearance {
    float renderSize, scale, fadeDistance; unsigned char alpha; unsigned long tint; Channel lighting; int meshEffect;
    std::vector<Prim*> all;
    Prim* AddPersistentPrimitive(unsigned long mesh) { Prim* p = new Prim; p->mesh = mesh; p->refreshes = 0; p->hasPacket = false; all.push_back(p); return p; }
};
static long g_LightingChannelDrawFilter = -1;
struct MultipleStaticMeshes {
    std::vector<Prim*> PrimHandles; std::vector<Entry> Meshes; bool bulletTime;
    bool OnAppearanceDraw(Appearance& tc, int reasons) {
        float size = tc.renderSize * tc.scale;
        float fade_distance = tc.fadeDistance;
        unsigned long i;
        if (PrimHandles.empty()) {
            for (i = 0; i < Meshes.size(); ++i) PrimHandles.push_back(tc.AddPersistentPrimitive(Meshes[i].Mesh));
        } else if (reasons == 1) {
            for (i = 0; i < PrimHandles.size(); ++i) ++PrimHandles[i]->refreshes;
            return true;
        }
        for (i = 0; i < PrimHandles.size(); ++i) {
            if (Meshes[i].Mesh == 0) continue;
            Channel channel;
            if (Meshes[i].OverrideLightingChannel) channel.Index0 = (unsigned char)Meshes[i].LightingChannel;
            else channel = tc.lighting;
            if (g_LightingChannelDrawFilter < 0 || channel.Index0 == 0) {
                float fade = fade_distance;
                if (Meshes[i].OverrideRenderFadeDistance) fade = Meshes[i].RenderFadeDistance;
                Packet p; p.mesh = Meshes[i].Mesh; p.size = size; p.fade = fade; p.lit = true; p.bullet = bulletTime;
                p.alpha = tc.alpha; p.tint = tc.tint; p.channel = channel; p.effect = tc.meshEffect;
                PrimHandles[i]->packet = p; PrimHandles[i]->hasPacket = true;
            }
        }
        return true;
    }
};
static int fails = 0;
static void check(bool c, const char* w) { if (!c) { ++fails; std::printf("FAIL %s\n", w); } }
static Entry E(unsigned long mesh, bool of, float fd, bool ol, long lc) { Entry e; e.Mesh = mesh; e.OverrideRenderFadeDistance = of; e.RenderFadeDistance = fd; e.OverrideLightingChannel = ol; e.LightingChannel = lc; return e; }
static Appearance A() { Appearance a; a.renderSize = 2.0f; a.scale = 1.5f; a.fadeDistance = 40.0f; a.alpha = 200; a.tint = 0x11223344; a.lighting.Index0 = 3; a.lighting.Index1 = 4; a.lighting.Blend = 5; a.meshEffect = 77; return a; }
int main() {
    MultipleStaticMeshes m; m.bulletTime = true;
    m.Meshes.push_back(E(10, false, 0, false, 0));      // appearance channel (index0 3), default fade
    m.Meshes.push_back(E(0, true, 5, true, 0));         // no mesh: never rebuilt
    m.Meshes.push_back(E(12, true, 7.5f, true, 9));     // override channel 9, override fade
    m.Meshes.push_back(E(13, false, 0, true, 0));       // override channel 0
    Appearance a = A();
    // first draw: one primitive per entry, then every entry with a mesh is built (filter -1)
    check(m.OnAppearanceDraw(a, 1), "returns true (first draw)");
    check(m.PrimHandles.size() == 4 && a.all.size() == 4 && a.all[2]->mesh == 12 && a.all[1]->mesh == 0, "one primitive per entry, entry mesh");
    check(a.all[0]->refreshes == 0, "first draw is not a position refresh even when reasons == 1");
    check(a.all[0]->hasPacket && !a.all[1]->hasPacket && a.all[2]->hasPacket && a.all[3]->hasPacket, "zero mesh skipped");
    Packet p0 = a.all[0]->packet, p2 = a.all[2]->packet;
    check(p0.size == 3.0f && p0.fade == 40.0f && p0.lit && p0.bullet && p0.alpha == 200 && p0.tint == 0x11223344 && p0.effect == 77, "packet fields");
    check(p0.channel.Index0 == 3 && p0.channel.Index1 == 4 && p0.channel.Blend == 5, "appearance channel copied whole");
    check(p2.fade == 7.5f && p2.channel.Index0 == 9 && p2.channel.Index1 == 0 && p2.channel.Blend == 0, "override fade / override channel");
    // position-only redraw: refresh every handle, rebuild nothing
    for (size_t k = 0; k < a.all.size(); ++k) a.all[k]->hasPacket = false;
    check(m.OnAppearanceDraw(a, 1), "returns true (refresh)");
    check(a.all[0]->refreshes == 1 && a.all[1]->refreshes == 1 && !a.all[0]->hasPacket && a.all.size() == 4, "refresh only");
    // filter >= 0: only entries whose channel index0 is 0 are drawn
    g_LightingChannelDrawFilter = 0;
    check(m.OnAppearanceDraw(a, 2), "returns true (filtered)");
    check(!a.all[0]->hasPacket && !a.all[1]->hasPacket && !a.all[2]->hasPacket && a.all[3]->hasPacket, "filter keeps index0 == 0 only");
    check(a.all.size() == 4 && a.all[3]->packet.channel.Index0 == 0 && a.all[3]->packet.fade == 40.0f, "no new primitives, channel 0 drawn");
    // appearance channel 0 passes the filter as well
    a.lighting.Index0 = 0; a.all[0]->hasPacket = false;
    m.OnAppearanceDraw(a, 0);
    check(a.all[0]->hasPacket && a.all[0]->packet.channel.Index1 == 4, "appearance channel with index0 0 passes the filter");
    if (fails == 0) std::printf("007e15c0_TEST PASS\n");
    return fails;
}
