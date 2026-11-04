// Behaviour test for NTCScriptedControl::CActionPlayCombatAnimation::OnActivate 0x00712A70: activating the scripted animation action marks it
// active (the base OnActivate) and removes the animation-blocking creature modes from the script's parent thing.
#include <cstdio>
struct CThing { int Modes; };
static void RemoveAnimationBlockingModes(CThing& thing) { thing.Modes &= ~3; }   // drops Stunned / Dazed
struct CTCScriptedControl { void* vptr_stand_in; CThing* ParentThing; CThing& GetParentThing() { return *ParentThing; } };
struct CActionBase {
    CTCScriptedControl* PTCScript; bool Activated; bool Terminated;
    CActionBase() : PTCScript(0), Activated(false), Terminated(false) {}
    virtual ~CActionBase() {}
    virtual void OnActivate() { Activated = true; }
    void SetScript(CTCScriptedControl* s) { PTCScript = s; }
    CThing& GetParentThing() { return PTCScript->GetParentThing(); }
};
struct CActionPlayCombatAnimation : CActionBase { void OnActivate() { CActionBase::OnActivate(); RemoveAnimationBlockingModes(GetParentThing()); } };
int main()
{
    CThing thing = { 7 };
    CTCScriptedControl script = { 0, &thing };
    CActionPlayCombatAnimation action; action.SetScript(&script);
    CActionBase* base = &action;
    base->OnActivate();
    if (!action.Activated || thing.Modes != 4) { std::printf("00712a70_TEST FAIL\n"); return 1; }
    std::printf("00712a70_TEST PASS\n");
    return 0;
}