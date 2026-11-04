// Behaviour test for NTCScriptedControl::CActionSummonerLightningAttackTarget::OnActivate 0x007122B0: activating the lightning attack
// gives its turn-to-face sub-action the same script and activates that sub-action; the attack action's own flag is left alone.
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
struct CActionTurnToFacePosition : CActionBase { };
struct CActionSummonerLightningAttackTarget : CActionBase {
    CActionTurnToFacePosition TurnToFaceAction;
    void OnActivate() { TurnToFaceAction.SetScript(PTCScript); TurnToFaceAction.OnActivate(); }
};
int main()
{
    CThing thing = { 0 };
    CTCScriptedControl script = { 0, &thing };
    CActionSummonerLightningAttackTarget action; action.SetScript(&script);
    CActionBase* base = &action;
    base->OnActivate();
    if (action.TurnToFaceAction.PTCScript != &script || !action.TurnToFaceAction.Activated || action.Activated) { std::printf("007122b0_TEST FAIL\n"); return 1; }
    std::printf("007122b0_TEST PASS\n");
    return 0;
}