#include <cstdio>
#include <stddef.h>
#include <new>
#include <string.h>
static char g_log[256]; static int g_n = 0;
static void Log(const char* s) { strcat(g_log, s); ++g_n; }
struct CComponentInterface {
    virtual void v000() { }
    virtual void v001() { }
    virtual void v002() { }
    virtual void v003() { }
    virtual void v004() { }
    virtual void v005() { }
    virtual void v006() { }
    virtual void v007() { }
    virtual void v008() { }
    virtual void v009() { }
    virtual void v010() { }
    virtual void v011() { }
    virtual void v012() { }
    virtual void v013() { }
    virtual void v014() { }
    virtual void v015() { }
    virtual void v016() { }
    virtual void v017() { }
    virtual void v018() { }
    virtual void v019() { }
    virtual void v020() { }
    virtual void v021() { }
    virtual void v022() { }
    virtual void v023() { }
    virtual void v024() { }
    virtual void v025() { }
    virtual void v026() { }
    virtual void v027() { }
    virtual void v028() { }
    virtual void v029() { }
    virtual void v030() { }
    virtual void v031() { }
    virtual void v032() { }
    virtual void v033() { }
    virtual void v034() { }
    virtual void v035() { }
    virtual void v036() { }
    virtual void v037() { }
    virtual void v038() { }
    virtual void v039() { }
    virtual void v040() { }
    virtual void v041() { }
    virtual void v042() { }
    virtual void v043() { }
    virtual void v044() { }
    virtual void v045() { }
    virtual void v046() { }
    virtual void v047() { }
    virtual void ChangeState(long s) { char b[32]; std::sprintf(b, "C%lx;", s); Log(b); }
    virtual void v049() { }
    virtual void v050() { }
    virtual void v051() { }
    virtual void v052() { }
    virtual void v053() { }
    virtual void v054() { }
    virtual void v055() { }
    virtual void v056() { }
    virtual void v057() { }
    virtual void v058() { }
    virtual void v059() { }
    virtual void v060() { }
    virtual void v061() { }
    virtual void v062() { }
    virtual void v063() { }
    virtual void v064() { }
    virtual void v065() { }
    virtual void v066() { }
    virtual void v067() { }
    virtual void v068() { }
    virtual void v069() { }
    virtual void v070() { }
    virtual void v071() { }
    virtual void v072() { }
    virtual void v073() { }
    virtual void v074() { }
    virtual void v075() { }
    virtual void v076() { }
    virtual void v077() { }
    virtual void v078() { }
    virtual void v079() { }
    virtual void v080() { }
    virtual void v081() { }
    virtual void v082() { }
    virtual void v083() { }
    virtual void v084() { }
    virtual void v085() { }
    virtual void v086() { }
    virtual void v087() { }
    virtual void v088() { }
    virtual void v089() { }
    virtual void v090() { }
    virtual void v091() { }
    virtual void v092() { }
    virtual void v093() { }
    virtual void v094() { }
    virtual void v095() { }
    virtual void v096() { }
    virtual void v097() { }
    virtual void v098() { }
    virtual void v099() { }
    virtual void v100() { }
    virtual void v101() { }
    virtual void v102() { }
    virtual void v103() { }
    virtual void v104() { }
    virtual void v105() { }
    virtual void v106() { }
    virtual void v107() { }
    virtual void v108() { }
    virtual void v109() { }
    virtual void v110() { }
    virtual void v111() { }
    virtual void v112() { }
    virtual void v113() { }
    virtual void v114() { }
    virtual void v115() { }
    virtual void v116() { }
    virtual void v117() { }
    virtual void v118() { }
    virtual void v119() { }
    virtual void v120() { }
    virtual void v121() { }
    virtual void v122() { }
    virtual void v123() { }
    virtual void v124() { }
    virtual void v125() { }
    virtual void v126() { }
    virtual void v127() { }
    virtual void v128() { }
    virtual void v129() { }
    virtual void v130() { }
    virtual void ExecuteActions(void* a) { char b[32]; std::sprintf(b, "X%p;", a); Log(b); }
    virtual void v132() { }
    virtual void v133() { }
};
struct CObserver_Retail {
    virtual void v0() { } virtual void ProcessEvent() { } virtual void v2() { }
    virtual void ObserveEvent(long e) { char b[32]; std::sprintf(b, "O%lx;", e); Log(b); }
    virtual void StopObservingEvent(long e) { char b[32]; std::sprintf(b, "S%lx;", e); Log(b); }
    virtual void v5() { }
    unsigned char _body[0x10];
};
struct CEventSinkInterface { virtual void v0() { } virtual void v1() { } virtual void v2() { } virtual void v3() { } };
struct CActionList;
struct CHoverable_Retail : CComponentInterface, CObserver_Retail, CEventSinkInterface {
    unsigned char _component[0x15c - 0x1c];
    long StateBeforeHovered; bool WasHovered; CActionList* PActionsOnHovered; CActionList* PActionsOnUnhovered;
    CHoverable_Retail(long owner) : StateBeforeHovered(owner), WasHovered(true), PActionsOnHovered(0), PActionsOnUnhovered(0) { Log("HovCtor;"); }
    void OnHovered() { Log("HovOn;"); }
    void OnUnhovered() { Log("HovOff;"); }
    void OnDeactivate() { Log("HovDeact;"); WasHovered = false; }
};
struct CClickable_Retail : CHoverable_Retail {
    long StateBeforeLeftClicked; bool WasLeftClicked; CActionList* PActionsOnLeftClicked; CActionList* PActionsOnLeftHeld; CActionList* PActionsOnLeftUnclicked;
    long StateBeforeRightClicked; bool WasRightClicked; CActionList* PActionsOnRightClicked; float InputDelay; float LastInputTime;
    CClickable_Retail(long owner) : CHoverable_Retail(owner), StateBeforeLeftClicked(0), WasLeftClicked(false), PActionsOnLeftClicked(0), PActionsOnLeftHeld(0), PActionsOnLeftUnclicked(0), StateBeforeRightClicked(0), WasRightClicked(false), PActionsOnRightClicked(0) { InitialiseInputDelay(); }
    void InitialiseInputDelay() { InputDelay = 0.25f; Log("Delay;"); }
    void OnLeftUnclicked() { ChangeState(StateBeforeLeftClicked); StopObservingEvent(0x1c); ExecuteActions(PActionsOnLeftUnclicked); }
    void OnHovered() { CHoverable_Retail::OnHovered(); ObserveEvent(0x1a); ObserveEvent(0x1f); ObserveEvent(0x1b); ObserveEvent(0x20); }
    void OnUnhovered() { CHoverable_Retail::OnUnhovered(); StopObservingEvent(0x1a); StopObservingEvent(0x1f); StopObservingEvent(0x1b); StopObservingEvent(0x20); }
    void OnDeactivate() { CHoverable_Retail::OnDeactivate(); WasLeftClicked = false; WasRightClicked = false; StopObservingEvent(0x1c); StopObservingEvent(0x1d); }
};
static bool Layout()
{
    return offsetof(CClickable_Retail, StateBeforeHovered) == 0x15c && offsetof(CClickable_Retail, StateBeforeLeftClicked) == 0x16c
        && offsetof(CClickable_Retail, WasLeftClicked) == 0x170 && offsetof(CClickable_Retail, PActionsOnLeftUnclicked) == 0x17c
        && offsetof(CClickable_Retail, WasRightClicked) == 0x184 && offsetof(CClickable_Retail, PActionsOnRightClicked) == 0x188 && sizeof(CClickable_Retail) == 0x194
        && (char*)(CObserver_Retail*)(CClickable_Retail*)0x1000 == (char*)0x1004 && (char*)(CEventSinkInterface*)(CClickable_Retail*)0x1000 == (char*)0x1018;
}
int main()
{
    unsigned char raw[sizeof(CClickable_Retail)]; CClickable_Retail* c = new (raw) CClickable_Retail(1); g_log[0] = 0; g_n = 0;
    c->WasLeftClicked = true; c->WasRightClicked = true; c->WasHovered = true;
    c->OnDeactivate();
    if (!Layout() || strcmp(g_log, "HovDeact;S1c;S1d;") != 0 || g_n != 3 || c->WasLeftClicked || c->WasRightClicked || c->WasHovered) { std::printf("0055af30_TEST FAIL\n"); return 1; }
    std::printf("0055af30_TEST PASS\n"); return 0;
}
