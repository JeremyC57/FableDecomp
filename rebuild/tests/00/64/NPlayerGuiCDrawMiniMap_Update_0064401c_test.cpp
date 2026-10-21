// behaviour test for 0064401c NPlayerGui::CDrawMiniMap::Update (host g++/VC)
#include <stdio.h>
#include <string.h>
struct CRGBColour { unsigned char B, G, R, A; CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; } CRGBColour(const CRGBColour& o) : B(o.B), G(o.G), R(o.R), A(o.A) {} };
static int g_starts = 0, g_updates = 0; static float g_lastIn, g_lastOut, g_lastElapsed; static unsigned char g_lastA;
struct CFadeInFadeOutBase {
    bool Active; float FadeInTime; float FadeOutTime; bool Closing; bool Opening; float OpenTimer; float CloseTimer;
    void Start(float in, float out, CRGBColour c) { ++g_starts; g_lastIn = in; g_lastOut = out; g_lastA = c.A; Active = true; Opening = true; Closing = false; }
    void Update(float e) { ++g_updates; g_lastElapsed = e; }
    void FadeOut() { Opening = false; Closing = true; }
};
struct CDisplayEngine { bool faded; bool IsScreenFullyFadedOut() { return faded; } };
struct CMainGameComponent { CDisplayEngine* m_pDisplayEngine; };
struct CWorld { bool showing; bool ready; bool MiniMapEnabled; bool MiniMapActive; long RegionLoadStage;
    bool IsDisplayingMiniMapOnRegionLoad() { return showing; } bool IsRegionLoadShowingMiniMap() { return ready; } };
struct CDrawMiniMap {
    CWorld* World; CMainGameComponent* Component; bool Enabled; bool Active; bool OldActive; bool RegionChange; CFadeInFadeOutBase FadeInOut; int activeCalls; bool lastActive;
    void SetAsActive(bool a) { ++activeCalls; lastActive = a; Active = a; }
    void Update(const float& elapsed) {
        CWorld* world;
        if (Component->m_pDisplayEngine->IsScreenFullyFadedOut() && World->IsDisplayingMiniMapOnRegionLoad() && (world = World)->IsRegionLoadShowingMiniMap()) {
            if (!RegionChange) { FadeInOut.Start(0.5f, 1.0f, CRGBColour(0, 0, 0, 255)); OldActive = Active; Active = true; RegionChange = true; }
            else if (!FadeInOut.Closing && world->RegionLoadStage == 8) FadeInOut.FadeOut();
        } else {
            if (RegionChange) { Active = OldActive; RegionChange = false; }
            else if (Active) { if (!FadeInOut.Active) FadeInOut.Start(0.5f, 0.5f, CRGBColour(0, 0, 0, 255)); }
            else if (FadeInOut.Active) FadeInOut.FadeOut();
        }
        FadeInOut.Update(elapsed);
        world = World;
        bool enabled = world->MiniMapEnabled;
        if (enabled != Enabled) { Enabled = enabled; if (enabled) SetAsActive(world->MiniMapActive); else SetAsActive(false); }
    }
};
int main() {
    CDisplayEngine de = { true }; CMainGameComponent comp = { &de }; CWorld w = { true, true, false, true, 3 };
    CDrawMiniMap m; memset(&m, 0, sizeof m); m.World = &w; m.Component = &comp; m.Active = false; m.Enabled = false;
    float dt = 0.25f;
    m.Update(dt);   // region-load branch: start fade, remember old active
    bool ok = g_starts == 1 && g_lastIn == 0.5f && g_lastOut == 1.0f && g_lastA == 255 && m.Active && m.RegionChange && !m.OldActive && g_updates == 1 && g_lastElapsed == 0.25f && m.activeCalls == 0;
    w.RegionLoadStage = 8; m.Update(dt);   // close once stage 8
    ok = ok && m.FadeInOut.Closing && !m.FadeInOut.Opening && g_starts == 1;
    de.faded = false; m.Update(dt);        // restore after region change
    ok = ok && !m.RegionChange && !m.Active;
    m.FadeInOut.Active = true; m.FadeInOut.Closing = false; m.Update(dt);   // inactive map with live fade: fade out
    ok = ok && m.FadeInOut.Closing;
    w.MiniMapEnabled = true; m.Update(dt);  // mirror the world's enable flag
    ok = ok && m.Enabled && m.activeCalls == 1 && m.lastActive == true;
    w.MiniMapEnabled = false; m.Update(dt);
    ok = ok && !m.Enabled && m.activeCalls == 2 && m.lastActive == false;
    printf(ok ? "0064401c_TEST PASS\n" : "0064401c_TEST FAIL\n");
    return ok ? 0 : 1;
}