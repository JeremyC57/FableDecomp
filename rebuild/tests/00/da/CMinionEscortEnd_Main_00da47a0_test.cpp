// Behaviour test for CMinionEscortEnd::Main @ 0x00DA47A0: the call sequence against a scripted game interface.
#include <cstdio>
#include <cstring>

static char g_log[256];
static void logs(const char* s) { std::strcat(g_log, s); }

struct CScriptThing {
    mutable int awareAfter;
    bool IsAwareOfHero() const { logs("a"); return awareAfter-- <= 0; }
};
struct CHandle {
    bool scripting;
    CHandle() : scripting(false) {}
    ~CHandle() { logs("~"); }
    bool IsScripting() { return scripting; }
    void Release() { logs("R"); }
};
struct CGame {
    int frames, startFailures, doneAt;
    CScriptThing hero;
    void NewScriptFrame() { ++frames; logs("f"); }
    bool StartScriptingEntity(const CScriptThing&, CHandle&, int priority) { logs(priority == 4 ? "S" : "?"); return startFailures-- <= 0; }
    const CScriptThing& GetHero() { return hero; }
    void EntitySetFacingAngleTowardsThing(const CScriptThing&, const CScriptThing&, bool instant) { logs(instant ? "?" : "F"); }
    void GiveThingBestEnemyTarget(const CScriptThing&, const CScriptThing&) { logs("T"); }
    void OpinionSourceSetAsAttentionGrabbing(const CScriptThing&, bool g) { logs(g ? "O" : "?"); }
};
struct CMinionEscortEnd {
    CGame* Game; CScriptThing Me;
    void RegisterFilter() { logs("r"); }
    bool IsDone() { return Game->frames >= Game->doneAt; }
    void Main();
};

void CMinionEscortEnd::Main()
{
    RegisterFilter();
    Game->NewScriptFrame();
    if (IsDone())
        return;
    CHandle handle;
    if (handle.IsScripting())
        handle.Release();
    while (!Game->StartScriptingEntity(Me, handle, 4)) {
        Game->NewScriptFrame();
        if (IsDone())
            return;
    }
    if (IsDone())
        return;
    Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
    while (!Me.IsAwareOfHero()) {
        Game->NewScriptFrame();
        if (IsDone())
            return;
    }
    if (IsDone())
        return;
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    Game->OpinionSourceSetAsAttentionGrabbing(Me, true);
    handle.Release();
    do {
        Game->NewScriptFrame();
    } while (!IsDone());
}

int main()
{
    CGame g; g.frames = 0; g.startFailures = 1; g.doneAt = 6;
    CMinionEscortEnd e; e.Game = &g; e.Me.awareAfter = 1;
    e.Main();
    int ok = std::strcmp(g_log, "rfSfSFafaTORfff~") == 0;
    g_log[0] = 0; g.frames = 0; g.startFailures = 5; g.doneAt = 3; e.Me.awareAfter = 0;
    e.Main();
    ok = ok && std::strcmp(g_log, "rfSfSf~") == 0;
    if (!ok) { std::printf("00da47a0_TEST FAIL\n"); return 1; }
    std::printf("00da47a0_TEST PASS\n");
    return 0;
}