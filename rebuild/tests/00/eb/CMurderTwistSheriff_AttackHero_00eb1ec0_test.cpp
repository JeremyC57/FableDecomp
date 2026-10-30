#include <cstdio>
#include <cstring>
// Behaviour test for 00eb1ec0 CMurderTwistSheriff::Init: the five interface calls in order, then the conditional handle releases.
static char g_log[256];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
class CScriptThing { public:
    int id; bool friends;
    void SetFriendsWithEverythingFlag(bool f) { friends = f; Log(f ? "F1" : "F0"); }
};
struct Handle {
    bool scripting; int released;
    bool IsScripting() { return scripting; }
    void Release() { ++released; scripting = false; }
};
class CGameScriptInterface { public:
    CScriptThing hero;
    virtual CScriptThing& GetHero() { Log("Hero"); return hero; }
    virtual void GiveThingBestEnemyTarget(const CScriptThing& t, const CScriptThing& target) const { Log(t.id == 7 && target.id == 99 ? "Enemy" : "EnemyBAD"); }
    virtual void EntitySetAsDamageable(const CScriptThing& t, bool d) const { Log(t.id == 7 && d ? "Dmg" : "DmgBAD"); }
    virtual void EntitySetAsKillable(const CScriptThing& t, bool k, bool b) const { Log(t.id == 7 && k && b ? "Kill" : "KillBAD"); }
    virtual void EntitySetAsUseMovementInActions(const CScriptThing& t, bool u) const { Log(t.id == 7 && u ? "Move" : "MoveBAD"); }
    virtual void SetIsPushableByHero(CScriptThing t, bool p) { Log(t.id == 7 && p ? "Push" : "PushBAD"); }
    virtual ~CGameScriptInterface() {}
};
struct CV_MurderTwistFinalScript { Handle SheriffHandle; };
struct CMurderTwistSheriff {
    CGameScriptInterface* Game;
    CScriptThing Me;
    CV_MurderTwistFinalScript* ParentClass;
    Handle MyEntityHandle;
    void Init();
};
void CMurderTwistSheriff::Init()
{
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    Game->EntitySetAsKillable(Me, true, true);
    Game->EntitySetAsDamageable(Me, true);
    Game->SetIsPushableByHero(Me, true);
    Game->EntitySetAsUseMovementInActions(Me, true);
    if (MyEntityHandle.IsScripting())
        MyEntityHandle.Release();
    Handle* handle = &ParentClass->SheriffHandle;
    if (handle->IsScripting())
        handle->Release();
}
int main()
{
    CGameScriptInterface g; g.hero.id = 99; g.hero.friends = false;
    CV_MurderTwistFinalScript parent; parent.SheriffHandle.scripting = true; parent.SheriffHandle.released = 0;
    CMurderTwistSheriff s; s.Game = &g; s.Me.id = 7; s.Me.friends = true; s.ParentClass = &parent;
    s.MyEntityHandle.scripting = false; s.MyEntityHandle.released = 0;
    g_log[0] = 0;
    s.Init();
    const char* want = "Hero;Enemy;Kill;Dmg;Push;Move;";
    bool ok = std::strcmp(g_log, want) == 0 && s.MyEntityHandle.released == 0;
    ok = ok && parent.SheriffHandle.released == 1;
    s.MyEntityHandle.scripting = true; g_log[0] = 0; s.Init();
    ok = ok && s.MyEntityHandle.released == 1;
    ok = ok && parent.SheriffHandle.released == 1;
    if (!ok) { std::printf("00eb1ec0_TEST FAIL\n"); return 1; }
    std::printf("00eb1ec0_TEST PASS\n");
    return 0;
}