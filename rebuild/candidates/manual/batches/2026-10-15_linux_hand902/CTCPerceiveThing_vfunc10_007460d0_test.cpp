#include <cstdio>
// Behaviour test for 0x007460D0: CTCPerceiveThing slot 10 forwards to slot 9 (FrameUpdate).
struct CTCPerceiveThing_Calls {
    int calls;
    CTCPerceiveThing_Calls() : calls(0) {}
    virtual void v00() {}
    virtual void v01() {}
    virtual void v02() {}
    virtual void v03() {}
    virtual void v04() {}
    virtual void v05() {}
    virtual void v06() {}
    virtual void v07() {}
    virtual bool v08() { return true; }
    virtual void FrameUpdate() { calls += 1; }
    virtual void vfunc_10();
};
void CTCPerceiveThing_Calls::vfunc_10() { FrameUpdate(); }

struct Derived : CTCPerceiveThing_Calls {
    virtual void FrameUpdate() { calls += 10; }
};

int main()
{
    CTCPerceiveThing_Calls a;
    Derived d;
    a.vfunc_10();
    a.vfunc_10();
    CTCPerceiveThing_Calls* p = &d;
    p->vfunc_10();
    if (a.calls != 2 || d.calls != 10) { std::printf("007460d0_TEST FAIL\n"); return 1; }
    std::printf("007460d0_TEST PASS\n");
    return 0;
}
