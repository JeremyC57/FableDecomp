// behaviour test for 0x007B6790 CTCCrateStack::RemoveNextCrate
#include <cstdio>
struct CThing { int id; };
struct Ptr { CThing* p; Ptr() : p(0) {} Ptr(CThing* t) : p(t) {} CThing* GetPItem() const { return p; } Ptr& operator=(const Ptr& o) { p = o.GetPItem(); return *this; } };
struct Stack {
    Ptr FirstCrate, SecondCrate;
    CThing* RemoveNextCrate()
    {
        CThing* crate = 0;
        Ptr* slot = &SecondCrate;
        if (slot->GetPItem() || (slot = &FirstCrate)->GetPItem()) { crate = slot->GetPItem(); *slot = Ptr(0); }
        return crate;
    }
};
int main()
{
    CThing a = { 1 }, b = { 2 };
    Stack s; s.FirstCrate = Ptr(&a); s.SecondCrate = Ptr(&b);
    bool ok = s.RemoveNextCrate() == &b && s.SecondCrate.p == 0 && s.FirstCrate.p == &a;
    ok = ok && s.RemoveNextCrate() == &a && s.FirstCrate.p == 0;
    ok = ok && s.RemoveNextCrate() == 0;
    if (!ok) { std::printf("007b6790_TEST FAIL\n"); return 1; }
    std::printf("007b6790_TEST PASS\n");
    return 0;
}