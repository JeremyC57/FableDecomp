// Behaviour test for CProgressDisplay::StopProgress @ 0x00499D90: the enabled/suppressed gate, the clear-to-black path
// (restoring the render state), the "Stopping progress display" frame with the black-screen override, and the release of
// all three texture sets plus the player GUI graphics.
#include <cstdio>
#include <vector>
#include <string>
struct Texture { int* Released; void Uninitialise() { ++*Released; } };
struct Display { bool Rendering; int Begins, Ends, Swaps, Boxes; };
struct Progress {
    bool Initialised, DebugDisplay, BlackScreen;
    std::vector<Texture> ProgressTextures, GuildSealTextures, LoadingScreenTextures;
    std::string LastRendered;
    bool BlackDuringRender;
    int GuiReleases;
    void RenderProgress(const char* text) { LastRendered = text; BlackDuringRender = BlackScreen; }
    void Stop(bool enabled, bool suppressed, bool showText, bool clear, Display* display, bool blackA, bool hasGui)
    {
        if (!enabled || suppressed)
            return;
        if (Initialised) {
            if (clear) {
                if (display) {
                    bool was = display->Rendering;
                    if (!was) { ++display->Begins; display->Rendering = true; }
                    ++display->Boxes;
                    ++display->Ends; display->Rendering = false;
                    ++display->Swaps;
                    if (was) { ++display->Begins; display->Rendering = true; }
                }
            } else if (!DebugDisplay && showText) {
                bool old = BlackScreen;
                if (blackA)
                    BlackScreen = true;
                RenderProgress("Stopping progress display");
                BlackScreen = old;
            }
        }
        unsigned int i;
        for (i = 0; i < ProgressTextures.size(); ++i) ProgressTextures[i].Uninitialise();
        ProgressTextures.clear();
        for (i = 0; i < GuildSealTextures.size(); ++i) GuildSealTextures[i].Uninitialise();
        GuildSealTextures.clear();
        for (i = 0; i < LoadingScreenTextures.size(); ++i) LoadingScreenTextures[i].Uninitialise();
        LoadingScreenTextures.clear();
        if (hasGui)
            ++GuiReleases;
        Initialised = false;
    }
};
int main()
{
    int released = 0;
    Texture t = { &released };
    Progress p;
    p.Initialised = true; p.DebugDisplay = false; p.BlackScreen = false; p.BlackDuringRender = false; p.GuiReleases = 0;
    p.ProgressTextures.assign(3, t); p.GuildSealTextures.assign(2, t); p.LoadingScreenTextures.assign(1, t);
    p.Stop(false, false, true, false, 0, false, true);
    bool ok = p.Initialised && released == 0;
    p.Stop(true, false, true, false, 0, true, true);
    ok = ok && p.LastRendered == "Stopping progress display" && p.BlackDuringRender && !p.BlackScreen &&
         released == 6 && p.ProgressTextures.empty() && p.GuiReleases == 1 && !p.Initialised;
    Display d = { true, 0, 0, 0, 0 };
    p.Initialised = true;
    p.Stop(true, false, false, true, &d, false, false);
    ok = ok && d.Boxes == 1 && d.Ends == 1 && d.Swaps == 1 && d.Begins == 1 && d.Rendering && p.GuiReleases == 1;
    if (!ok) { std::printf("00499d90_TEST FAIL\n"); return 1; }
    std::printf("00499d90_TEST PASS\n");
    return 0;
}
