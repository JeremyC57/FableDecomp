#include <cstdio>
#include <vector>
// Behaviour model of CTCBetting::FrameUpdate (0x007E4E30): mode switching, events, start frame, handle clearing.
struct Prim { long count; int notified; int deleted; };
struct Handle {
    Prim* p;
    void Clear() {
        if (p && p->count == 1) p->notified++;
        if (p) { if (--p->count == 0) p->deleted++; p = 0; }
        p = 0;
    }
};
static bool g_bettingActive; static long g_bettingAmount = 77; static long g_bettingStartDelaySeconds = 1; static long g_fps = 30;
static long g_serverFrame = 100; static int g_mode = 0; static int g_events = 0; static int g_lastEventData = -1; static int g_texts = 0;
static bool g_hasDef = true;
struct Betting {
    bool Active; std::vector<Handle> BetHandles, MoneyHandles; long FrameToStart;
    void FrameUpdate() {
        if (Active == g_bettingActive) return;
        Active = g_bettingActive;
        if (Active) {
            if (g_mode != 39) g_mode = 39;
            g_bettingAmount = 0;
            if (g_hasDef) { g_texts++; g_events++; g_lastEventData = 1; }
            FrameToStart = g_serverFrame + g_bettingStartDelaySeconds * g_fps;
        } else {
            if (g_mode == 39) { g_mode = 0; g_events++; g_lastEventData = 0; }
            for (std::vector<Handle>::iterator it = BetHandles.begin(); it != BetHandles.end(); ++it) it->Clear();
            for (std::vector<Handle>::iterator it = MoneyHandles.begin(); it != MoneyHandles.end(); ++it) it->Clear();
        }
    }
};
int main()
{
    Betting b; b.Active = false; b.FrameToStart = -1;
    b.FrameUpdate();
    bool ok = g_events == 0 && b.FrameToStart == -1;
    g_bettingActive = true; b.FrameUpdate();
    ok = ok && g_mode == 39 && g_bettingAmount == 0 && g_events == 1 && g_lastEventData == 1 && b.FrameToStart == 130 && g_texts == 1;
    Prim a = {1, 0, 0}, c = {2, 0, 0};
    Handle h1 = {&a}, h2 = {&c}, h3 = {0};
    b.BetHandles.push_back(h1); b.MoneyHandles.push_back(h2); b.MoneyHandles.push_back(h3);
    g_bettingActive = false; b.FrameUpdate();
    ok = ok && g_mode == 0 && g_events == 2 && g_lastEventData == 0;
    ok = ok && a.notified == 1 && a.deleted == 1 && a.count == 0 && c.notified == 0 && c.count == 1 && c.deleted == 0;
    ok = ok && b.BetHandles[0].p == 0 && b.MoneyHandles[0].p == 0 && b.MoneyHandles[1].p == 0;
    g_hasDef = false; g_bettingActive = true; g_serverFrame = 10; b.FrameUpdate();
    ok = ok && g_events == 2 && b.FrameToStart == 40 && g_mode == 39;
    if (!ok) { std::printf("007e4e30_TEST FAIL\n"); return 1; }
    std::printf("007e4e30_TEST PASS\n");
    return 0;
}