// behaviour test for 0x007B66C0 CTCCrateStack::AddCrate
#include <cstdio>
struct CThing { int id; };
struct Ptr { CThing* p; Ptr() : p(0) {} Ptr(CThing* t) : p(t) {} CThing* GetPItem() const { return p; } Ptr& operator=(const Ptr& o) { p = o.GetPItem(); return *this; } };
struct Stack {
    Ptr FirstCrate, SecondCrate;
    void AddCrate(CThing* c)
    {
        if (!FirstCrate.GetPItem()) FirstCrate = Ptr(c);
        else if (!SecondCrate.GetPItem()) SecondCrate = Ptr(c);
    }
};
int main()
{
    CThing a = { 1 }, b = { 2 }, c = { 3 };
    Stack s; s.AddCrate(&a);
    bool ok = s.FirstCrate.p == &a && s.SecondCrate.p == 0;
    s.AddCrate(&b);
    ok = ok && s.FirstCrate.p == &a && s.SecondCrate.p == &b;
    s.AddCrate(&c);
    ok = ok && s.FirstCrate.p == &a && s.SecondCrate.p == &b;
    if (!ok) { std::printf("007b66c0_TEST FAIL\n"); return 1; }
    std::printf("007b66c0_TEST PASS\n");
    return 0;
}