// Behaviour test for 0x00E7BB30 CFishermanWasp::Main: filter registered and destroyed, a frame, then (unless done)
// the hero given as Me's enemy target and frames run until done; the handle is destroyed once.
#include <cstdio>
static int g_frames = 0, g_filters = 0, g_filterDtors = 0, g_handles = 0, g_handleDtors = 0, g_targets = 0, g_doneAfter = 0, g_isDoneCalls = 0;
struct CScriptThing { int id; };
struct CGameScriptInterface {
    CScriptThing hero; const CScriptThing* lastThing; const CScriptThing* lastTarget;
    void NewScriptFrame() const { ++g_frames; }
    const CScriptThing& GetHero() const { return hero; }
    void GiveThingBestEnemyTarget(const CScriptThing& thing, const CScriptThing& target) const
    { ++g_targets; const_cast<CGameScriptInterface*>(this)->lastThing = &thing; const_cast<CGameScriptInterface*>(this)->lastTarget = &target; }
};
struct CIsThingAlive {
    CScriptThing m_thing;
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { ++g_filters; }
    ~CIsThingAlive() { ++g_filterDtors; }
};
struct CScriptGameResourceObjectScriptedThingBase {
    void* Data; void* Info;
    CScriptGameResourceObjectScriptedThingBase() : Data(0), Info(0) { ++g_handles; }
    ~CScriptGameResourceObjectScriptedThingBase() { ++g_handleDtors; }
};
class CFishermanWasp {
public:
    CGameScriptInterface* Game;
    CScriptThing Me;
    int registered;
    void RegisterFilter(CIsThingAlive& filter) { registered = filter.m_thing.id; }
    bool IsDone() { return ++g_isDoneCalls > g_doneAfter; }
    void Main();
};
void CFishermanWasp::Main()
{
    {
        CIsThingAlive filter(Me);
        RegisterFilter(filter);
    }
    Game->NewScriptFrame();
    if (IsDone())
        return;
    CScriptGameResourceObjectScriptedThingBase seh;
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    do {
        Game->NewScriptFrame();
    } while (!IsDone());
}
int main()
{
    CGameScriptInterface gsi; gsi.hero.id = 1; gsi.lastThing = gsi.lastTarget = 0;
    CFishermanWasp w; w.Game = &gsi; w.Me.id = 7; w.registered = 0;
    int fail = 0;
    g_doneAfter = 0;                       // done immediately
    w.Main();
    if (w.registered != 7 || g_filters != 1 || g_filterDtors != 1 || g_frames != 1 || g_handles || g_targets) fail |= 1;
    g_frames = 0; g_isDoneCalls = 0; g_doneAfter = 3;
    w.Main();                              // 1 frame, target, then frames until IsDone() is true on its 4th call
    if (g_targets != 1 || gsi.lastThing != &w.Me || gsi.lastTarget != &gsi.hero || g_frames != 4 || g_handles != 1 || g_handleDtors != 1) fail |= 2;
    if (fail) { std::printf("00e7bb30_TEST FAIL\n"); return 1; }
    std::printf("00e7bb30_TEST PASS\n");
    return 0;
}