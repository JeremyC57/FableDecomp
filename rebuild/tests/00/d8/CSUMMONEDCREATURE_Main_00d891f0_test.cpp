// Behaviour test for 0x00d891f0 CSUMMONED_CREATURE::Main (Hero Souls Nostro)
#include <cstdio>

static int g_log[64]; static int g_n = 0;
static void Log(int v) { if (g_n < 64) g_log[g_n++] = v; }

struct CScriptThing {
    int id;
    CScriptThing(int i) : id(i) { }
    CScriptThing(const CScriptThing& o) : id(o.id) { Log(1); }
    ~CScriptThing() { if (id == 2) Log(2); }
};
struct CQ_SunnyvaleMasterData { bool CollectedSoulFromNostro; };
struct CGameScriptInterface {
    CScriptThing hero; CQ_SunnyvaleMasterData* data; int frames, collectAt;
    const CScriptThing* targeter; const CScriptThing* target; const CScriptThing* removed; bool ra, rb;
    CGameScriptInterface() : hero(5), frames(0), collectAt(3), targeter(0), target(0), removed(0) { }
    void NewScriptFrame() { Log(3); if (++frames == collectAt) data->CollectedSoulFromNostro = true; }
    CScriptThing& GetHero() { return hero; }
    void GiveThingBestEnemyTarget(const CScriptThing& t, const CScriptThing& e) { Log(4); targeter = &t; target = &e; }
    void RemoveThing(const CScriptThing& t, bool a, bool b) { Log(5); removed = &t; ra = a; rb = b; }
};
struct CIsThingAlive {
    CScriptThing Thing;
    CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
};
struct CSUMMONED_CREATURE {
    CGameScriptInterface* Game; CScriptThing Me; CQ_SunnyvaleMasterData* PMasterData; int terminateAt, polls; int predicateId;
    CSUMMONED_CREATURE() : Me(2), terminateAt(-1), polls(0), predicateId(0) { }
    void SetPredicate(const CIsThingAlive& p) { Log(6); predicateId = p.Thing.id; }
    bool IsActiveThreadTerminating() { return ++polls == terminateAt; }
    void Main()
    {
        {
            CIsThingAlive predicate(Me);
            SetPredicate(predicate);
        }
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
        while (!PMasterData->CollectedSoulFromNostro) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->RemoveThing(Me, false, true);
    }
};

int main()
{
    CQ_SunnyvaleMasterData md; md.CollectedSoulFromNostro = false;
    CGameScriptInterface gsi; gsi.data = &md;
    CSUMMONED_CREATURE s; s.Game = &gsi; s.PMasterData = &md;
    s.Main();
    // copy, register, copy dtor, frame, target, frame, frame (collected), remove
    int expect[] = { 1, 6, 2, 3, 4, 3, 3, 5 };
    bool ok = g_n == 8;
    for (int i = 0; ok && i < 8; ++i) ok = g_log[i] == expect[i];
    if (!ok || s.predicateId != 2 || gsi.targeter != &s.Me || gsi.target != &gsi.hero || gsi.removed != &s.Me || gsi.ra || !gsi.rb) {
        std::printf("00d891f0_TEST FAIL\n"); return 1;
    }
    // terminated right after the first frame: no target, no removal
    g_n = 0; md.CollectedSoulFromNostro = false;
    CGameScriptInterface gsi2; gsi2.data = &md; CSUMMONED_CREATURE s2; s2.Game = &gsi2; s2.PMasterData = &md; s2.terminateAt = 1;
    s2.Main();
    if (gsi2.targeter != 0 || gsi2.removed != 0 || gsi2.frames != 1) { std::printf("00d891f0_TEST FAIL\n"); return 1; }
    std::printf("00d891f0_TEST PASS\n");
    return 0;
}