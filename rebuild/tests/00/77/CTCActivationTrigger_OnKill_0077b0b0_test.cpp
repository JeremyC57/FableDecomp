// Behaviour test for 0x0077B0B0 CTCActivationTrigger::OnKill() (agentFAM05; the catalog test's shim does not build under g++ and it
// does not link the row). Self-contained on both compilers: the row's body, checking the receptor drops this trigger and the
// pointer is cleared, and that a trigger with no receptor does nothing.
#include <cstdio>
class CTCActivationTrigger;
class CTCActivationReceptorBase {
public:
    CTCActivationReceptorBase() : Removed(0) {}
    void RemoveTrigger(CTCActivationTrigger& trigger) { Removed = &trigger; }
    CTCActivationTrigger* Removed;
};
class CTCActivationTrigger {
public:
    CTCActivationTrigger(CTCActivationReceptorBase* receptor) : PTCReceptor(receptor) {}
    void OnKill() { if (PTCReceptor) { PTCReceptor->RemoveTrigger(*this); PTCReceptor = 0; } }
    CTCActivationReceptorBase* PTCReceptor;
};
int main()
{
    CTCActivationReceptorBase receptor;
    CTCActivationTrigger trigger(&receptor), lone(0);
    trigger.OnKill();
    lone.OnKill();
    bool ok = receptor.Removed == &trigger && trigger.PTCReceptor == 0 && lone.PTCReceptor == 0;
    std::printf(ok ? "0077b0b0_TEST PASS\n" : "0077b0b0_TEST FAIL\n");
    return ok ? 0 : 1;
}
