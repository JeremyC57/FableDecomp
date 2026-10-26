// Behaviour test for CThing::IsFreeForDeletion @ 0x004C7500: every live TC must agree; removed TCs are skipped.
#include <cstdio>
class CTCBase { public:
    bool ToBeRemoved; bool ok; int calls;
    CTCBase(bool removed, bool okay) : ToBeRemoved(removed), ok(okay), calls(0) { }
    virtual bool IsFreeForDeletion() { ++calls; return ok; }
};
struct SComponentPair { long first; CTCBase* second; };
class CThing { public:
    SComponentPair* m_componentsFirst;
    SComponentPair* m_componentsLast;
    bool IsFreeForDeletion()
    {
        for (SComponentPair* it = m_componentsFirst; it != m_componentsLast; ++it) {
            CTCBase* tc = it->second;
            if (!tc->ToBeRemoved && !tc->IsFreeForDeletion())
                return false;
        }
        return true;
    }
};
int main()
{
    CTCBase a(false, true), b(true, false), c(false, false);
    SComponentPair v[3] = { { 1, &a }, { 2, &b }, { 3, &c } };
    CThing t; t.m_componentsFirst = v; t.m_componentsLast = v + 3;
    if (t.IsFreeForDeletion() || a.calls != 1 || b.calls != 0 || c.calls != 1) { std::printf("004c7500_TEST FAIL\n"); return 1; }
    t.m_componentsLast = v + 2;
    if (!t.IsFreeForDeletion()) { std::printf("004c7500_TEST FAIL\n"); return 1; }
    std::printf("004c7500_TEST PASS\n");
    return 0;
}