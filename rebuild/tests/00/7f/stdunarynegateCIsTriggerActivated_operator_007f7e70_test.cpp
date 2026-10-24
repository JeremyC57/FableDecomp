#include <cstdio>
#include <cmath>
// Behaviour test: std::unary_negate<CIsTriggerActivated>::operator() -- true for a thing whose activation trigger is NOT activated.
#define CHECK(c) if (!(c)) { std::printf("007f7e70_TEST FAIL\n"); return 1; }
struct CTCActivationTrigger { bool Activated; };
struct CThing { const CTCActivationTrigger* Trigger; void PeekTC(const CTCActivationTrigger** out) const { *out = Trigger; } };
struct CIsTriggerActivated {
    typedef CThing* argument_type;
    bool operator()(CThing* thing) const { const CTCActivationTrigger* t; thing->PeekTC(&t); return t->Activated; }
};
template <class P> struct unary_negate {
    P _M_pred;
    explicit unary_negate(const P& x) : _M_pred(x) { }
    bool operator()(const typename P::argument_type& x) const { return !_M_pred(x); }
};
int main()
{
    CTCActivationTrigger on, off; on.Activated = true; off.Activated = false;
    CThing a, b; a.Trigger = &on; b.Trigger = &off;
    unary_negate<CIsTriggerActivated> notActivated((CIsTriggerActivated()));
    CThing* pa = &a; CThing* pb = &b;
    CHECK(!notActivated(pa) && notActivated(pb))
    std::printf("007f7e70_TEST PASS\n");
    return 0;
}