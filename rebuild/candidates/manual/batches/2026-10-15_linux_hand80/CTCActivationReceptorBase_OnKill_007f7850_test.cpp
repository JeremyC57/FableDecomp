#include <cstdio>
#include <cstring>

struct CThing { int informed; struct CTCActivationReceptorBase* lastReceptor; };
struct CTCActivationReceptorBase;
struct CInformTriggerOfReceptorDeath {
    CTCActivationReceptorBase* m_pReceptor;
    CInformTriggerOfReceptorDeath(CTCActivationReceptorBase* r) : m_pReceptor(r) { }
    void operator()(CThing* trigger) { ++trigger->informed; trigger->lastReceptor = m_pReceptor; }
};
template<class It, class Fn> Fn for_each_model(It first, It last, Fn f) { for (; first != last; ++first) f(*first); return f; }
struct CThingPtrVector { CThing** m_first; CThing** m_last; CThing** m_end; CThing** begin() { return m_first; } CThing** end() { return m_last; } };
struct CTCActivationReceptorBase {
    bool Activated; CThingPtrVector Triggers;
    void OnKill() { for_each_model(Triggers.begin(), Triggers.end(), CInformTriggerOfReceptorDeath(this)); }
};
int main()
{
    CThing a, b; a.informed = b.informed = 0; a.lastReceptor = b.lastReceptor = 0;
    CThing* list[2]; list[0] = &a; list[1] = &b;
    CTCActivationReceptorBase r; r.Activated = false; r.Triggers.m_first = list; r.Triggers.m_last = list + 2; r.Triggers.m_end = list + 2;
    r.OnKill();
    if (a.informed != 1 || b.informed != 1 || a.lastReceptor != &r || b.lastReceptor != &r) { std::printf("007f7850_TEST FAIL\n"); return 1; }
    r.Triggers.m_last = list; r.OnKill();
    if (a.informed != 1 || b.informed != 1) { std::printf("007f7850_TEST FAIL\n"); return 1; }
    std::printf("007f7850_TEST PASS\n");
    return 0;
}
