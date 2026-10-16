// Behaviour test for 00dcf920: CArtefact::OnPredicateFail -- erase the first crate whose thing matches; refcounts balance.
#include <cstdio>
struct TBlock { long refs; };
struct TCrate { void* thing; TBlock* info; };
struct TLocal {
    TCrate c;
    TLocal(const TCrate& o) : c(o) { if (c.info) ++c.info->refs; }
    ~TLocal() { if (c.info) --c.info->refs; }
    bool IsThing(void* t) const { return c.thing == t; }
};
struct TParent {
    TCrate list[4]; int n;
    void erase(int i) { for (int j = i; j + 1 < n; ++j) list[j] = list[j + 1]; --n; }
};
struct TArtefact {
    void* m_pThing; TParent* ParentClass;
    void OnPredicateFail()
    {
        for (int i = 0; i != ParentClass->n; ++i) {
            TLocal crate(ParentClass->list[i]);
            if (crate.IsThing(m_pThing)) { ParentClass->erase(i); return; }
        }
    }
};
int main()
{
    int a, b, c; TBlock ba = { 1 }, bb = { 1 }, bc = { 1 };
    TParent p; p.n = 3;
    p.list[0].thing = &a; p.list[0].info = &ba; p.list[1].thing = &b; p.list[1].info = &bb; p.list[2].thing = &c; p.list[2].info = &bc;
    TArtefact art; art.ParentClass = &p; art.m_pThing = &b;
    art.OnPredicateFail();
    if (p.n != 2 || p.list[0].thing != &a || p.list[1].thing != &c || ba.refs != 1 || bb.refs != 1) { std::printf("00dcf920_TEST FAIL\n"); return 1; }
    int z; art.m_pThing = &z; art.OnPredicateFail();
    if (p.n != 2) { std::printf("00dcf920_TEST FAIL\n"); return 1; }
    std::printf("00dcf920_TEST PASS\n");
    return 0;
}
