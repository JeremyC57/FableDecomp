#include <cstdio>
#include <cmath>
// Behaviour test: std::_For_each<CThing**, CInformTriggerOfReceptorDeath> -- clears the receptor pointer of every trigger in the range and returns the functor.
#define CHECK(c) if (!(c)) { std::printf("007f7b70_TEST FAIL\n"); return 1; }
class CTCActivationReceptorBase { public: int id; };
class CTCActivationTrigger { public: CTCActivationReceptorBase* Receptor; void SetPTCReceptor(CTCActivationReceptorBase* r) { Receptor = r; } };
class CThing { public: CTCActivationTrigger* Trigger; void GetTC(CTCActivationTrigger** out) { *out = Trigger; } };
class CInformTriggerOfReceptorDeath { public:
    CTCActivationReceptorBase* PReceptor;
    void operator()(CThing* thing) { CTCActivationTrigger* trigger; thing->GetTC(&trigger); trigger->SetPTCReceptor(0); }
};
template <class It, class F> F ForEach(It first, It last, F func) { for (; first != last; ++first) func(*first); return func; }
int main()
{
    CTCActivationReceptorBase rec; rec.id = 1;
    CTCActivationTrigger t1, t2, t3; t1.Receptor = &rec; t2.Receptor = &rec; t3.Receptor = &rec;
    CThing a, b, c; a.Trigger = &t1; b.Trigger = &t2; c.Trigger = &t3;
    CThing* things[3] = { &a, &b, &c };
    CInformTriggerOfReceptorDeath f; f.PReceptor = &rec;
    CInformTriggerOfReceptorDeath r = ForEach(things, things + 2, f);
    CHECK(r.PReceptor == &rec && t1.Receptor == 0 && t2.Receptor == 0 && t3.Receptor == &rec)
    std::printf("007f7b70_TEST PASS\n");
    return 0;
}