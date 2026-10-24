#include <cstdio>
#include <cmath>
// Behaviour test: CTCActivationReceptorRelay::SetTriggerState -- forwards the state to the parent thing's activation trigger.
#define CHECK(c) if (!(c)) { std::printf("007f8160_TEST FAIL\n"); return 1; }
struct CTCActivationTrigger { int calls; bool last; void OnTriggerActivate(bool s) { ++calls; last = s; } };
struct CThing { CTCActivationTrigger* Trigger; void GetTC(CTCActivationTrigger** out) { *out = Trigger; } };
struct CTCActivationReceptorRelay {
    CThing* ParentThing;
    void SetTriggerState(bool state) { CTCActivationTrigger* trigger; ParentThing->GetTC(&trigger); trigger->OnTriggerActivate(state); }
};
int main()
{
    CTCActivationTrigger t; t.calls = 0; t.last = false;
    CThing th; th.Trigger = &t;
    CTCActivationReceptorRelay r; r.ParentThing = &th;
    r.SetTriggerState(true); CHECK(t.calls == 1 && t.last)
    r.SetTriggerState(false); CHECK(t.calls == 2 && !t.last)
    std::printf("007f8160_TEST PASS\n");
    return 0;
}