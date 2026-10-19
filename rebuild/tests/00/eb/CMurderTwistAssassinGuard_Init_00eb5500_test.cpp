// behaviour copy of CMurderTwistAssassinGuard::Init
#include <cstdio>
static int g_seq[16]; static int g_n = 0; static int g_args[16][3]; static int g_friends = -1;
struct CScriptThing { int id; void SetFriendsWithEverythingFlag(bool f) { g_friends = f; g_seq[g_n++] = 67; } };
struct CGameScriptInterface {
    CScriptThing hero;
    void rec(int s, int a = 0, int b = 0) { g_args[g_n][0] = a; g_args[g_n][1] = b; g_seq[g_n++] = s; }
    const CScriptThing& GetHero() const { return hero; }
    void RemoveThing(const CScriptThing&, bool a, bool b) { rec(108, a, b); }
    void SetIsThingForcePushable(const CScriptThing&, bool p) { rec(745, p); }
    void SetIsPushableByHero(CScriptThing, bool p) { rec(844, p); }
    void EntitySetAsUseMovementInActions(const CScriptThing&, bool u) { rec(529, u); }
    void EntitySetAsKillable(const CScriptThing&, bool k, bool b) { rec(517, k, b); }
    void EntitySetAsDamageable(const CScriptThing&, bool d) { rec(516, d); }
    void EntityForceToLookAtThing(const CScriptThing&, const CScriptThing& t) { rec(498, t.id); }
    void SetThingPersistent(const CScriptThing&, bool p) { rec(477, p); }
};
struct CParent { long m_startedFlag48; };
struct CMurderTwistAssassinGuard {
    CGameScriptInterface* Game; CScriptThing Me; CParent* ParentClass; unsigned long NextLine; bool HasBeenSpokenTo;
    void Init()
    {
        if (ParentClass->m_startedFlag48 == 0) {
            Game->RemoveThing(Me, false, true);
            return;
        }
        Game->SetIsThingForcePushable(Me, false);
        Game->SetIsPushableByHero(Me, false);
        Game->EntitySetAsUseMovementInActions(Me, false);
        Game->EntitySetAsKillable(Me, false, true);
        Game->EntitySetAsDamageable(Me, false);
        Game->EntityForceToLookAtThing(Me, Game->GetHero());
        Me.SetFriendsWithEverythingFlag(true);
        Game->SetThingPersistent(Me, true);
        NextLine = 1;
        HasBeenSpokenTo = false;
    }
};
int main()
{
    CGameScriptInterface g; g.hero.id = 1; CParent p; p.m_startedFlag48 = 0;
    CMurderTwistAssassinGuard a; a.Game = &g; a.Me.id = 3; a.ParentClass = &p; a.NextLine = 0; a.HasBeenSpokenTo = true;
    a.Init();
    if (g_n != 1 || g_seq[0] != 108 || g_args[0][0] != 0 || g_args[0][1] != 1 || a.NextLine != 0) { std::printf("00eb5500_TEST FAIL\n"); return 1; }
    g_n = 0; p.m_startedFlag48 = 1; a.Init();
    static const int order[8] = { 745, 844, 529, 517, 516, 498, 67, 477 };
    for (int i = 0; i < 8; ++i) if (g_seq[i] != order[i]) { std::printf("00eb5500_TEST FAIL\n"); return 1; }
    if (g_n != 8 || g_friends != 1 || g_args[7][0] != 1 || a.NextLine != 1 || a.HasBeenSpokenTo) { std::printf("00eb5500_TEST FAIL\n"); return 1; }
    std::printf("00eb5500_TEST PASS\n");
    return 0;
}