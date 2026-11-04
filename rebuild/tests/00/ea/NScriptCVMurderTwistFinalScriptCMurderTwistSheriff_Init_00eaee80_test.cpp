// behaviour copy of CMurderTwistSheriff::Init
#include <cstdio>
static int g_seq[16]; static int g_n = 0; static int g_args[16][3];
class CScriptThing { public: int id; };
class CGameScriptInterface { public:
    CScriptThing hero;
    void rec(int s, int a = 0, int b = 0, int c = 0) { g_args[g_n][0] = a; g_args[g_n][1] = b; g_args[g_n][2] = c; g_seq[g_n++] = s; }
    const CScriptThing& GetHero() const { return hero; }
    void SetIsThingForcePushable(const CScriptThing&, bool p) { rec(745, p); }
    void SetIsPushableByHero(CScriptThing t, bool p) { rec(844, p, t.id); }
    void EntitySetAsUseMovementInActions(const CScriptThing&, bool u) { rec(529, u); }
    void EntitySetAsKillable(const CScriptThing&, bool k, bool b) { rec(517, k, b); }
    void EntitySetAsDamageable(const CScriptThing&, bool d) { rec(516, d); }
    void EntityForceToLookAtThing(const CScriptThing&, const CScriptThing& t) { rec(498, t.id); }
    void SetThingHasInformation(const CScriptThing&, bool a, bool b, bool c) { rec(360, a, b, c); }
};
struct CMurderTwistSheriff {
    CGameScriptInterface* Game; CScriptThing Me; unsigned long NextLine;
    bool HasGivenReminderToHero, HasBeingRumbledCutscenePlayed, HasExpressedSuspicionToHero, HasSaidWellDoneToHero;
    void Init()
    {
        Game->SetIsThingForcePushable(Me, false);
        Game->SetIsPushableByHero(Me, false);
        Game->EntitySetAsUseMovementInActions(Me, false);
        Game->EntitySetAsKillable(Me, false, true);
        Game->EntitySetAsDamageable(Me, false);
        Game->EntityForceToLookAtThing(Me, Game->GetHero());
        Game->SetThingHasInformation(Me, false, false, false);
        HasGivenReminderToHero = false; HasBeingRumbledCutscenePlayed = false; HasExpressedSuspicionToHero = false; HasSaidWellDoneToHero = false;
        NextLine = 1;
    }
};
int main()
{
    CGameScriptInterface g; g.hero.id = 1;
    CMurderTwistSheriff s; s.Game = &g; s.Me.id = 5; s.NextLine = 0; s.HasGivenReminderToHero = s.HasSaidWellDoneToHero = true;
    s.Init();
    static const int order[7] = { 745, 844, 529, 517, 516, 498, 360 };
    for (int i = 0; i < 7; ++i) if (g_seq[i] != order[i]) { std::printf("00eaee80_TEST FAIL\n"); return 1; }
    if (g_n != 7 || g_args[1][1] != 5 || g_args[3][0] != 0 || g_args[3][1] != 1 || g_args[5][0] != 1 || s.NextLine != 1
        || s.HasGivenReminderToHero || s.HasSaidWellDoneToHero)
    { std::printf("00eaee80_TEST FAIL\n"); return 1; }
    std::printf("00eaee80_TEST PASS\n");
    return 0;
}