#include <cstdio>
#include <cstring>
struct CCharString {
    const char* p;
    CCharString(const char* t, long = -1) : p(t) {}
    CCharString(const CCharString& o) : p(o.p) {}
    bool is(const char* t) const { return std::strcmp(p, t) == 0; }
};
static int g_fail = 0;
static void check(bool c, int line) { if (!c) { if (!g_fail) std::printf("check failed at line %d\n", line); g_fail = 1; } }
#define CHECK(c) check((c), __LINE__)
// Scripted world: frame counter drives where the hero is; IsActiveThreadTerminating after frame 40.
static int g_frame = 0;
struct GameCalls {
    void NewScriptFrame() { ++g_frame; }
    bool IsRegionLoaded(const CCharString& r) const
    {
        if (r.is("GreatwoodBanditToll")) return g_frame >= 5 && g_frame < 10;
        if (r.is("Greatwood")) return g_frame >= 12;
        return false;
    }
};
struct CV_BanditTollScript {
    GameCalls* Game;
    bool BanditsAngered;
    bool TollPaid;
    bool IsActiveThreadTerminating() { return g_frame >= 40; }
    void ResetToll();
};

void CV_BanditTollScript::ResetToll()
{
    while (!IsActiveThreadTerminating()) {
        while (!Game->IsRegionLoaded("GreatwoodBanditToll")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            if ((TollPaid || BanditsAngered) && (Game->IsRegionLoaded("Greatwood") || Game->IsRegionLoaded("GreatwoodLake"))) {
                if (IsActiveThreadTerminating()) return;
                TollPaid = false;
                BanditsAngered = false;
            }
        }
        if (IsActiveThreadTerminating()) return;
        while (Game->IsRegionLoaded("GreatwoodBanditToll") != false) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        Game->NewScriptFrame();
    }
}

int main()
{
    GameCalls g; CV_BanditTollScript s; s.Game = &g; s.TollPaid = true; s.BanditsAngered = true;
    s.ResetToll();
    CHECK(!s.TollPaid && !s.BanditsAngered && g_frame == 40);
    g_frame = 0; s.TollPaid = false; s.BanditsAngered = true;
    s.ResetToll();
    CHECK(!s.BanditsAngered);
    if (g_fail) { std::printf("00e480d0_TEST FAIL\n"); return 1; }
    std::printf("00e480d0_TEST PASS\n");
    return 0;
}