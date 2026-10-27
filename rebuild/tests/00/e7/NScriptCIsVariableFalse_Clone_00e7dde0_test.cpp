#include <cstdio>
#include <cstring>
// Behaviour test: a copy of the source's logic over a mock model (calls are recorded, not executed).
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static const char* g_str = "";
struct CPredicateBase { virtual bool operator()() const = 0; virtual CPredicateBase* Clone() const = 0; virtual ~CPredicateBase() { } };
struct CIsVariableFalse : CPredicateBase {
    const bool* Variable;
    CIsVariableFalse(const bool& v) : Variable(&v) { }
    virtual bool operator()() const { return !*Variable; }
    virtual CPredicateBase* Clone() const { return new CIsVariableFalse(*Variable); }
};
struct CIsThingAlive : CPredicateBase {
    const bool* Alive;
    CIsThingAlive(const bool& a) : Alive(&a) { }
    virtual bool operator()() const { return *Alive; }
    virtual CPredicateBase* Clone() const { return new CIsThingAlive(*this); }
};
template <class P1, class P2> struct CPredicate_And : CPredicateBase {
    P1 Predicate1; P2 Predicate2;
    CPredicate_And(const P1& a, const P2& b) : Predicate1(a), Predicate2(b) { }
    virtual bool operator()() const { return Predicate1() && Predicate2(); }
    virtual CPredicateBase* Clone() const { return new CPredicate_And(*this); }
};

int main()
{
    g_log[0] = 0;
    bool ok = true;
    bool v = false; CIsVariableFalse p(v);
    CPredicateBase* c = p.Clone();
    ok = c != 0 && (*c)() && ((CIsVariableFalse*)c)->Variable == &v;
    v = true; ok = ok && !(*c)();
    delete c;
    if (!ok) { std::printf("00e7dde0_TEST FAIL\n"); return 1; }
    std::printf("00e7dde0_TEST PASS\n");
    return 0;
}