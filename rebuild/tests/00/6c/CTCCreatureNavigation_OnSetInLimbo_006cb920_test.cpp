// Behaviour test for CTCCreatureNavigation::OnSetInLimbo (0x006CB920): interrupts the navigator whatever the flag.
#include <cstdio>
class CNavigatorBase { public:
    int Interrupts;
    virtual ~CNavigatorBase() {}
    virtual void Interrupt() { ++Interrupts; }
};
class CTCCreatureNavigation { public:
    CNavigatorBase* PNavigator;
    virtual ~CTCCreatureNavigation() {}
    virtual void OnSetInLimbo(bool) { PNavigator->Interrupt(); }
};
int main()
{
    CNavigatorBase n; n.Interrupts = 0;
    CTCCreatureNavigation c; c.PNavigator = &n;
    c.OnSetInLimbo(true);
    c.OnSetInLimbo(false);
    if (n.Interrupts != 2) { std::printf("006cb920_TEST FAIL\n"); return 1; }
    std::printf("006cb920_TEST PASS\n");
    return 0;
}