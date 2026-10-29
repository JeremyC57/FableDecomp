// Behaviour test for 0x00D86A30 NScript::CQ_HeroSoulsGuildmasterScript::CHSG_DistanceChecker::Main: wait until the hero is
// within HS_GMLongDistance (DistanceReached = 1), then within HS_GMShortDistance (DistanceReached = 2); a terminating thread
// returns at once.
#include <cstdio>

class CScriptThing { public: float Pos; };
class CScriptDef { public: long HS_GMShortDistance; long HS_GMLongDistance; };
static CScriptDef g_def;
static CScriptDef* pGlobalScriptDef = &g_def;

class CGameScriptInterface { public:
    CScriptThing Hero; float Step; int Frames;
    void NewScriptFrame() const { CGameScriptInterface* self = const_cast<CGameScriptInterface*>(this); self->Hero.Pos -= Step; ++self->Frames; }
    CScriptThing& GetHero() { return Hero; }
};
static bool IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float d)
{
    float x = a.Pos - b.Pos; if (x < 0) x = -x; return x < d;
}
struct CQ_HeroSoulsGuildmasterScript { long DistanceReached; };
struct CHSG_DistanceChecker {
    CGameScriptInterface* Game; CScriptThing Me; CQ_HeroSoulsGuildmasterScript* ParentClass;
    int TermAt, TermCalls;
    bool IsActiveThreadTerminating() const { CHSG_DistanceChecker* s = const_cast<CHSG_DistanceChecker*>(this); return s->TermAt && ++s->TermCalls >= s->TermAt; }
    void Main()
    {
        while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), (float)pGlobalScriptDef->HS_GMLongDistance)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        ParentClass->DistanceReached = 1;
        while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), (float)pGlobalScriptDef->HS_GMShortDistance)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        ParentClass->DistanceReached = 2;
    }
};

int main()
{
    g_def.HS_GMShortDistance = 5; g_def.HS_GMLongDistance = 20;
    CGameScriptInterface game; game.Hero.Pos = 50.0f; game.Step = 1.0f; game.Frames = 0;
    CQ_HeroSoulsGuildmasterScript quest; quest.DistanceReached = 0;
    CHSG_DistanceChecker c; c.Game = &game; c.Me.Pos = 0.0f; c.ParentClass = &quest; c.TermAt = 0; c.TermCalls = 0;
    c.Main();
    int ok = quest.DistanceReached == 2 && game.Hero.Pos == 4.0f && game.Frames == 46;
    // stop after the first stage: the long range is reached, then the thread terminates in the second wait
    game.Hero.Pos = 21.0f; game.Frames = 0; quest.DistanceReached = 0; c.TermAt = 4; c.TermCalls = 0;
    c.Main();
    ok = ok && quest.DistanceReached == 1 && game.Frames == 3;
    // terminating at once: no stage is reached
    game.Hero.Pos = 30.0f; quest.DistanceReached = 0; c.TermAt = 1; c.TermCalls = 0;
    c.Main();
    ok = ok && quest.DistanceReached == 0;
    if (!ok) { std::printf("00d86a30_TEST FAIL\n"); return 1; }
    std::printf("00d86a30_TEST PASS\n");
    return 0;
}