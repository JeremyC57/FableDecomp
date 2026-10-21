// Behaviour test for 00e242d0 (CQR_EscortTraderScript::ExpressionEnableFollow): when the trader's handle is scripting a follow on the hero
// the trader is re-taken and the follow stopped; after two frames the trader's state and the interface calls match
// the follow expression; a terminating thread stops early.
#include <cstdio>
#include <cstring>

struct Handle { bool scripting, following; int releases, stops; };
struct Game { int frames, termAt; bool startOk; int starts; bool info0, emote, region, followCalls, stopCalls, marker; };
struct Script {
    Game* g; bool TraderFollowing, DetFollow, DetWait, RegionChange; long QuestStage;
    bool Term() { return g->frames >= g->termAt; }
    void Run(Handle& h)
    {
        if (h.scripting && h.following) {
            if (Term()) return;
            if (h.scripting) { h.scripting = false; ++h.releases; }
            while (!(++g->starts, g->startOk)) { ++g->frames; if (Term()) return; }
            h.scripting = true;
            if (Term()) return;
            h.following = false; ++h.stops;
            if (h.scripting) { h.scripting = false; ++h.releases; }
        }
        ++g->frames; if (Term()) return;
        ++g->frames; if (Term()) return;
        DetFollow = false; DetWait = false;
#if 1
        TraderFollowing = true; RegionChange = false; QuestStage = 2;
        g->marker = true; g->info0 = false; g->followCalls = true; g->emote = true; g->region = true;
#else
        TraderFollowing = false; QuestStage = 3;
        g->info0 = true; g->stopCalls = true; g->emote = false; g->region = false;
#endif
    }
};
int main()
{
    Game g; std::memset(&g, 0, sizeof g); g.termAt = 100; g.startOk = true; g.emote = !0; g.region = g.emote;
    Script s; std::memset(&s, 0, sizeof s); s.g = &g; s.DetFollow = s.DetWait = true; s.RegionChange = true; s.TraderFollowing = false;
    Handle h = { true, true, 0, 0 };
    s.Run(h);
    int ok = h.releases == 2 && h.stops == 1 && g.frames == 2 && !s.DetFollow && !s.DetWait;
#if 1
    if (!s.TraderFollowing || s.QuestStage != 2 || s.RegionChange || !g.emote || !g.region || !g.followCalls) ok = 0;
#else
    if (s.TraderFollowing || s.QuestStage != 3 || !g.info0 || g.emote || g.region || !g.stopCalls) ok = 0;
#endif
    Handle idle = { false, false, 0, 0 }; s.QuestStage = 0; g.termAt = 3; s.Run(idle);
    if (idle.releases != 0 || s.QuestStage != 0) ok = 0;
    if (!ok) { std::printf("00e242d0_TEST FAIL\n"); return 1; }
    std::printf("00e242d0_TEST PASS\n");
    return 0;
}
