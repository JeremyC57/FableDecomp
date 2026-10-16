#include <cstdio>
struct C { int stage; unsigned int uidLo, uidHi; int availMask;
    bool IsChild() const { return stage == 1; }
    bool Avail(int t) const { return (availMask >> t) & 1; } };
struct F { C* Source; int Type;
    bool operator()(C* t) const {
        return ((t->IsChild() && Source->IsChild()) || (!t->IsChild() && !Source->IsChild()))
            && (t->uidLo != Source->uidLo || t->uidHi != Source->uidHi) && t->Avail(Type); } };
int main()
{
    C src = { 0, 1, 0, 0 };
    C a = { 0, 2, 0, 2 };
    F f = { &src, 1 };
    bool ok = f(&a);
    a.stage = 1; ok = ok && !f(&a);
    src.stage = 1; ok = ok && f(&a);
    a.uidLo = 1; ok = ok && !f(&a);
    a.uidHi = 1; ok = ok && f(&a);
    f.Type = 0; ok = ok && !f(&a);
    if (!ok) { std::printf("006a7430_TEST FAIL\n"); return 1; }
    std::printf("006a7430_TEST PASS\n");
    return 0;
}
