#include <cstdio>
// Behaviour test for 0x007460E0: CTCPerceiveThing slot 8 always answers true.
struct CTCPerceiveThing_Slot8 {
    virtual bool vfunc_8();
};
bool CTCPerceiveThing_Slot8::vfunc_8() { return true; }

int main()
{
    CTCPerceiveThing_Slot8 a, b;
    CTCPerceiveThing_Slot8* p = &b;
    if (!a.vfunc_8() || p->vfunc_8() != true) { std::printf("007460e0_TEST FAIL\n"); return 1; }
    std::printf("007460e0_TEST PASS\n");
    return 0;
}