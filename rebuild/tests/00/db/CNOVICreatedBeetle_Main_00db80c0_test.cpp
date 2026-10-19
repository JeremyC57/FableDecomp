#include <stdio.h>
// Behaviour test for CNOVI_CreatedBeetle::Main (0x00DB80C0): register the alive functor, wait one frame, then wait out a
// 5-tick timer (one script frame per tick) and remove the beetle unless the script finished meanwhile.
static int g_things = 0, g_frames = 0, g_timers = 0, g_removed = 0, g_functors = 0; static long g_time = 0;
struct CScriptThing {                                  // 12 bytes
    unsigned char _body[12];
    CScriptThing() { _body[0] = 0; ++g_things; }
    CScriptThing(const CScriptThing& other) { _body[0] = other._body[0]; ++g_things; }
    ~CScriptThing() { --g_things; }
};
struct CScriptThingFunctor_00ef7ab0 {                  // vtable 0x012C2FE8: an is-alive functor holding a script thing
    virtual void v00() {}
    CScriptThing m_thing;                              // +0x04
    CScriptThingFunctor_00ef7ab0(const CScriptThing& thing) : m_thing(thing) { }
    ~CScriptThingFunctor_00ef7ab0() { }
};
struct CGameScriptInterface {
    virtual void v000() {} virtual void v001() {} virtual void v002() {} virtual void v003() {} virtual void v004() {} virtual void v005() {} virtual void v006() {}
    virtual void NewScriptFrame() const { ++g_frames; }
    virtual void v008() {} virtual void v009() {} virtual void v010() {} virtual void v011() {} virtual void v012() {} virtual void v013() {} virtual void v014() {} virtual void v015() {}
    virtual void v016() {} virtual void v017() {} virtual void v018() {} virtual void v019() {} virtual void v020() {} virtual void v021() {} virtual void v022() {} virtual void v023() {}
    virtual void v024() {} virtual void v025() {} virtual void v026() {} virtual void v027() {} virtual void v028() {} virtual void v029() {} virtual void v030() {} virtual void v031() {}
    virtual void v032() {} virtual void v033() {} virtual void v034() {} virtual void v035() {} virtual void v036() {} virtual void v037() {} virtual void v038() {} virtual void v039() {}
    virtual void v040() {} virtual void v041() {} virtual void v042() {} virtual void v043() {} virtual void v044() {} virtual void v045() {} virtual void v046() {} virtual void v047() {}
    virtual void v048() {} virtual void v049() {} virtual void v050() {} virtual void v051() {} virtual void v052() {} virtual void v053() {} virtual void v054() {} virtual void v055() {}
    virtual void v056() {} virtual void v057() {} virtual void v058() {} virtual void v059() {} virtual void v060() {} virtual void v061() {} virtual void v062() {} virtual void v063() {}
    virtual void v064() {} virtual void v065() {} virtual void v066() {} virtual void v067() {} virtual void v068() {} virtual void v069() {} virtual void v070() {} virtual void v071() {}
    virtual void v072() {} virtual void v073() {} virtual void v074() {} virtual void v075() {} virtual void v076() {} virtual void v077() {} virtual void v078() {} virtual void v079() {}
    virtual void v080() {} virtual void v081() {} virtual void v082() {} virtual void v083() {} virtual void v084() {} virtual void v085() {} virtual void v086() {}
    virtual long RegisterTimer() { ++g_timers; return 42; }
    virtual void DeregisterTimer(long timer) { if (timer == 42) --g_timers; }
    virtual void SetTimer(long, long time) { g_time = time; }
    virtual long GetTimer(long) const { if (g_time > 0) --g_time; return g_time; }
    virtual void v091() {} virtual void v092() {} virtual void v093() {} virtual void v094() {} virtual void v095() {} virtual void v096() {} virtual void v097() {} virtual void v098() {}
    virtual void v099() {} virtual void v100() {} virtual void v101() {} virtual void v102() {} virtual void v103() {} virtual void v104() {} virtual void v105() {} virtual void v106() {}
    virtual void v107() {}
    virtual void RemoveThing(const CScriptThing&, bool a, bool b) const { if (a && b) ++g_removed; }
};
CGameScriptInterface* g_pGameScriptInterface;
struct CTimer {                                        // 4 bytes: an index into the global script timers
    long TimerIndex;
    CTimer(long time) { TimerIndex = g_pGameScriptInterface->RegisterTimer(); g_pGameScriptInterface->SetTimer(TimerIndex, time); }
    ~CTimer() { g_pGameScriptInterface->DeregisterTimer(TimerIndex); }
    long GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }
};
struct CQ_NewOakValeIntroScript;
struct CQ_SunnyvaleMasterData;
struct CNOVI_CreatedBeetle {
    void*                     __vftable;               // +0x00
    CGameScriptInterface*     Game;                    // +0x04
    CScriptThing              Me;                      // +0x08
    CQ_NewOakValeIntroScript* ParentClass;             // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;             // +0x18
    void RegisterFunctor(CScriptThingFunctor_00ef7ab0&) { ++g_functors; }
    int doneAt;
    bool IsDone() { return g_frames >= doneAt; }
    void Main();
};

void CNOVI_CreatedBeetle::Main()
{
    {
        CScriptThingFunctor_00ef7ab0 functor(Me);
        RegisterFunctor(functor);
    }
    Game->NewScriptFrame();
    if (IsDone())
        return;
    CTimer timer(5);
    while (timer.GetTime() != 0) {
        Game->NewScriptFrame();
        if (IsDone())
            return;
    }
    if (IsDone())
        return;
    Game->RemoveThing(Me, true, true);
}

static int Run(int doneAt)
{
    CGameScriptInterface game; g_pGameScriptInterface = &game;
    CNOVI_CreatedBeetle b; b.Game = &game; b.doneAt = doneAt;
    g_frames = 0; g_removed = 0; g_timers = 0;
    b.Main();
    if (g_timers != 0 || g_things != 1) return -1;
    return g_frames * 10 + g_removed;
}
int main()
{
    int ok = 1;
    if (Run(1000) != 51) ok = 0;      // 1 frame + 4 timer frames (GetTime 4,3,2,1,0), then removed
    if (Run(1) != 10) ok = 0;         // done after the first frame
    if (Run(3) != 30) ok = 0;         // done inside the timer loop
    if (g_functors != 3) ok = 0;
    if (ok) { printf("00db80c0_TEST PASS\n"); return 0; }
    printf("00db80c0_TEST FAIL\n"); return 1;
}