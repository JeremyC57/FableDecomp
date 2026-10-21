#include <cstdio>
#include <cstring>
#include <string>
// Behaviour model of CBriarRose::Init: one spawned WatchForAttack thread on Me, then six interface calls in order.
static std::string g_log;
struct CCharString {
    std::string s;
    CCharString(const char* t, int = -1) : s(t) {}
};
static CCharString operator+(const CCharString& a, const CCharString& b) { return CCharString((a.s + b.s).c_str()); }
static int g_refs = 1;
struct CScriptThing {
    int id;
    CScriptThing(int i) : id(i) {}
    CScriptThing(const CScriptThing& o) : id(o.id) { ++g_refs; }
    ~CScriptThing() { --g_refs; }
};
struct CSpawned { CCharString name; unsigned long id; CScriptThing par; CSpawned(const CCharString& n, unsigned long i, CScriptThing p) : name(n), id(i), par(p) {} };
struct CParent {
    CSpawned* added; std::string region;
    void Add(CSpawned* f, const CCharString& r) { added = f; region = r.s; g_log += "add;"; }
};
struct CGame {
    void SetThingHasInformation(const CScriptThing& t, bool a, bool b, bool c) const { char x[64]; std::sprintf(x, "info%d%d%d%d;", t.id, a, b, c); g_log += x; }
    void EntitySheatheWeapons(const CScriptThing& t, bool s) const { char x[64]; std::sprintf(x, "sheathe%d%d;", t.id, s); g_log += x; }
    void EntitySetAsOpinionSource(const CScriptThing&, const CCharString& n) const { g_log += "op:" + n.s + ";"; }
    void EntitySetAsKillable(const CScriptThing& t, bool k, bool b) const { char x[64]; std::sprintf(x, "kill%d%d%d;", t.id, k, b); g_log += x; }
    void SetIsPushableByHero(CScriptThing t, bool p) { char x[64]; std::sprintf(x, "push%d%d;", t.id, p); g_log += x; }
    void MiniMapAddMarker(const CScriptThing&, const CCharString& n) const { g_log += "map:" + n.s + ";"; }
};
struct CBriarRose {
    CGame* Game; CScriptThing Me; CParent* ParentClass;
    CBriarRose() : Me(7) {}
    void Init()
    {
        ParentClass->Add(new CSpawned(CCharString("ParentClass.", -1) + CCharString("WatchForAttack", -1), 0, Me), CCharString("", -1));
        Game->SetThingHasInformation(Me, false, true, false);
        Game->EntitySheatheWeapons(Me, false);
        { CCharString source("OPINION_SOURCE_BRIAR_ROSE_LOUD", -1); Game->EntitySetAsOpinionSource(Me, source); }
        Game->EntitySetAsKillable(Me, false, true);
        Game->SetIsPushableByHero(Me, false);
        { CCharString marker("HUD_ORB_QUEST_VIGNETTE", -1); Game->MiniMapAddMarker(Me, marker); }
    }
};
int main()
{
    CGame g; CParent p; CBriarRose b; b.Game = &g; b.ParentClass = &p;
    b.Init();
    const char* want = "add;info7010;sheathe70;op:OPINION_SOURCE_BRIAR_ROSE_LOUD;kill701;push70;map:HUD_ORB_QUEST_VIGNETTE;";
    if (g_log != want || p.added->name.s != "ParentClass.WatchForAttack" || p.added->id != 0 || p.added->par.id != 7
        || p.region != "" || g_refs != 2) {
        std::printf("00e9dac0_TEST FAIL\n"); return 1;
    }
    delete p.added;
    std::printf("00e9dac0_TEST PASS\n");
    return 0;
}