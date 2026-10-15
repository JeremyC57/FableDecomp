// Behaviour test for CThing::ValidateThing @ 0x004C7470: false on the first live TC that refuses, removed TCs skipped.
#include <cstdio>
struct CTCBase {
    bool ToBeRemoved; bool ok; int calls; long seen;
    CTCBase(bool removed, bool okay) : ToBeRemoved(removed), ok(okay), calls(0), seen(0) { }
    virtual bool ValidateTC(long context) { ++calls; seen = context; return ok; }
};
struct SComponentPair { long first; CTCBase* second; };
struct CThing {
    SComponentPair* m_componentsFirst;
    SComponentPair* m_componentsLast;
    bool ValidateThing(long context)
    {
        for (SComponentPair* it = m_componentsFirst; it != m_componentsLast; ++it) {
            CTCBase* tc = it->second;
            if (!tc->ToBeRemoved && !tc->ValidateTC(context))
                return false;
        }
        return true;
    }
};
int main()
{
    CTCBase a(false, true), b(true, false), c(false, false), d(false, true);
    SComponentPair v[4] = { { 1, &a }, { 2, &b }, { 3, &c }, { 4, &d } };
    CThing t; t.m_componentsFirst = v; t.m_componentsLast = v + 4;
    if (t.ValidateThing(42) || a.calls != 1 || a.seen != 42 || b.calls != 0 || c.calls != 1 || d.calls != 0) {
        std::printf("004c7470_TEST FAIL\n"); return 1;
    }
    t.m_componentsLast = v + 2;
    if (!t.ValidateThing(5)) { std::printf("004c7470_TEST FAIL\n"); return 1; }
    t.m_componentsLast = v;
    if (!t.ValidateThing(5)) { std::printf("004c7470_TEST FAIL\n"); return 1; }
    std::printf("004c7470_TEST PASS\n");
    return 0;
}