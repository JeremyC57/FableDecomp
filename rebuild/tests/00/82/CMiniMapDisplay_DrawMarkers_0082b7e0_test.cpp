// Behaviour test for CMiniMapDisplay::DrawMarkers 0x0082B7E0: a copy of the model's logic (marker ordering, numbering,
// flash fade open/close, hero auto marker) over plain stand-in state.
#include <cstdio>
#include <list>
#include <vector>
struct Fade { bool Active, Closing, Opening; float FadeIn, FadeOut, OpenTimer, CloseTimer; unsigned long Colour;
    void Open(float a, float b, unsigned long c) { Active = true; FadeIn = a; FadeOut = b; Closing = false; Opening = true; OpenTimer = 0.0f; CloseTimer = b; Colour = c; }
    void Update(float) { if (Opening) Opening = false; }
    bool IsOpen() const { return Active && !Opening && !Closing; }
    void Close() { Opening = false; Closing = true; } };
struct Marker { int thing; long GraphicIndex; bool AutoMarker; };
static std::vector<long> g_graphics;
static bool IsHeroMarker(const Marker& m) { const std::vector<long>& d = g_graphics; return m.GraphicIndex == d[0]; }
struct Display {
    std::vector<int> handles; std::list<Marker> Markers; Fade fade; void* PRegion;
    std::vector<int> drawnThing, drawnIndex; int autoThing; long autoGraphic;
    void DrawMarker(const Marker& m, long index) { drawnThing.push_back(m.thing); drawnIndex.push_back(index); }
    void AddAutoMarker(int thing, long g) { autoThing = thing; autoGraphic = g; }
    void DrawMarkers(bool loading, bool fadedOut, bool paused, int hero, bool heroDead) {
        handles.clear(); handles.resize(Markers.size());
        if (loading || fadedOut) return;
        if (!paused) { if (!fade.Active) fade.Open(0.5f, 0.5f, 0xff000000ul); fade.Update(0.1f); if (fade.IsOpen()) fade.Close(); }
        long index = 0; bool heroDrawn = false; std::list<Marker>::const_iterator it;
        for (it = Markers.begin(); it != Markers.end(); ++it) { const Marker& m = *it; if (IsHeroMarker(m)) { DrawMarker(m, index); ++index; heroDrawn = true; } }
        for (it = Markers.begin(); it != Markers.end(); ++it) { const Marker& m = *it; if (!IsHeroMarker(m) && !m.AutoMarker) { DrawMarker(m, index); ++index; } }
        for (it = Markers.begin(); it != Markers.end(); ++it) { const Marker& m = *it; if (!IsHeroMarker(m) && m.AutoMarker) { DrawMarker(m, index); ++index; } }
        if (!heroDrawn && PRegion) { if (hero && !heroDead) AddAutoMarker(hero, g_graphics[0]); }
    } };
int main()
{
    g_graphics.push_back(7); g_graphics.push_back(8);
    Display d; d.fade.Active = false; d.PRegion = &d; d.autoThing = 0; d.autoGraphic = 0;
    Marker a = {1, 3, true}, b = {2, 4, false}, c = {3, 7, false};
    d.Markers.push_back(a); d.Markers.push_back(b); d.Markers.push_back(c);
    d.DrawMarkers(false, false, false, 99, false);
    bool ok = d.handles.size() == 3 && d.drawnThing.size() == 3 && d.drawnThing[0] == 3 && d.drawnThing[1] == 2 && d.drawnThing[2] == 1
        && d.drawnIndex[2] == 2 && d.autoThing == 0 && d.fade.Closing && d.fade.Active;
    Display e; e.fade.Active = false; e.PRegion = &e; e.autoThing = 0; e.autoGraphic = 0; e.Markers.push_back(b);
    e.DrawMarkers(false, false, true, 99, false);
    ok = ok && e.autoThing == 99 && e.autoGraphic == 7 && !e.fade.Active && e.drawnThing.size() == 1;
    Display f; f.fade.Active = false; f.PRegion = &f; f.autoThing = 0; f.autoGraphic = 0; f.Markers.push_back(b);
    f.DrawMarkers(true, false, false, 99, false);
    ok = ok && f.drawnThing.empty() && f.handles.size() == 1 && f.autoThing == 0;
    if (!ok) { std::printf("0082b7e0_TEST FAIL\n"); return 1; }
    std::printf("0082b7e0_TEST PASS\n");
    return 0;
}