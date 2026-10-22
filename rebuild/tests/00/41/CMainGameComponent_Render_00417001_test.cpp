// Behaviour test for CMainGameComponent::Render (0x00417001): with a forced-render count below the frame rate the count is
// raised to the frame rate and catch-up rendering is used; otherwise the live interpolation is clamped to [0,1], copied
// into the paused/bullet-time sets, saved as the last info, and the frame is rendered and timed only when the client is
// ready (or the override flag is set); an unready client bumps the unable-to-render counter.
#include <cstdio>

struct Set { float GT, WF; };
struct Info { Set Current, Last, PausedCurrent, PausedLast, BulletCurrent, BulletLast; int ServerTime; };
struct Game {
    int forceFrames, fps, serverFrame, unable, noRender, renders, forced;
    bool paused, clientReady, overrideFlag, rendered;
    double presentTime, now, duration;
    Info last;
};
static double Clamp(double v, double lo, double hi) { return v < lo ? lo : v > hi ? hi : v; }
static void Render(Game& g)
{
    if (g.forceFrames > 0) {
        if (g.forceFrames < g.fps) g.forceFrames = g.fps;
        g.forced = g.forceFrames; g.rendered = true;
        return;
    }
    Info info = Info();
    if (!g.paused) {
        info.Last = g.last.Current; info.PausedLast = g.last.PausedCurrent; info.ServerTime = g.serverFrame;
        info.Current.WF = (float)Clamp(g.presentTime / (1.0f / g.fps) - g.serverFrame, 0.0, 1.0);
        info.Current.GT = 0.5f;
        info.PausedCurrent = info.Current; info.BulletCurrent = info.Current;
    } else {
        info.Last = g.last.Last; info.Current = g.last.Current; info.PausedLast = g.last.PausedCurrent;
        info.BulletCurrent = g.last.BulletCurrent; info.ServerTime = g.serverFrame;
        info.PausedCurrent.WF = (float)Clamp(g.presentTime / (1.0f / g.fps) - g.serverFrame, 0.0, 1.0);
        info.PausedCurrent.GT = 0.5f;
    }
    g.last = info;
    if (!g.clientReady) ++g.unable;
    if (g.overrideFlag || g.clientReady) {
        double start = g.now;
        g.now += 2.0;
        ++g.renders;
        g.duration = g.now - start;
        ++g.noRender; g.unable = 0; g.rendered = true;
    }
}

int main()
{
    Game g = Game();
    g.forceFrames = 3; g.fps = 10;
    Render(g);
    bool ok = g.forced == 10 && g.rendered;
    Game h = Game();
    h.fps = 10; h.serverFrame = 4; h.presentTime = 0.45; h.clientReady = false;
    Render(h);   // 4.5 - 4 = 0.5, client not ready: no render
    ok = ok && h.last.Current.WF > 0.49f && h.last.Current.WF < 0.51f && h.last.BulletCurrent.WF == h.last.Current.WF
         && h.unable == 1 && h.renders == 0 && !h.rendered;
    h.presentTime = 0.9; h.clientReady = true;
    Render(h);   // clamps to 1, renders, resets the counter
    ok = ok && h.last.Current.WF == 1.0f && h.renders == 1 && h.unable == 0 && h.duration == 2.0 && h.rendered && h.noRender == 1;
    h.paused = true; h.presentTime = 0.1;
    Render(h);   // paused: current held, paused interpolate clamped to 0
    ok = ok && h.last.Current.WF == 1.0f && h.last.PausedCurrent.WF == 0.0f;
    std::printf(ok ? "00417001_TEST PASS\n" : "00417001_TEST FAIL\n");
    return ok ? 0 : 1;
}