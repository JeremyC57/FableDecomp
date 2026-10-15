// Test for 006cba80 CTCCreatureNavigation::SetNavigatorToType: interrupt + switch only when the type differs.
#include <cstdio>
struct CNavigatorBase {
    unsigned char NavigatorType; int InterruptCalls;
    void Interrupt() { ++InterruptCalls; }
};
struct CTCCreatureNavigation {
    CNavigatorBase* PNavigator;
    CNavigatorBase** first; CNavigatorBase** last;
    static CNavigatorBase** FindNavigator(CNavigatorBase** f, CNavigatorBase** l, long type)
    {
        for (; f != l; ++f)
            if ((*f)->NavigatorType == type)
                return f;
        return l;
    }
    void SetNavigatorToType(long type)
    {
        if (PNavigator->NavigatorType != type) {
            PNavigator->Interrupt();
            PNavigator = *FindNavigator(first, last, type);
        }
    }
};
int main()
{
    CNavigatorBase a = { 0, 0 }, b = { 1, 0 }, c = { 2, 0 };
    CNavigatorBase* list[4] = { &a, &b, &c, &a };       // list[3] stands in for the slot at last
    CTCCreatureNavigation n = { &a, list, list + 3 };
    n.SetNavigatorToType(0);                            // same type: nothing
    if (a.InterruptCalls != 0 || n.PNavigator != &a) { std::printf("006cba80_TEST FAIL\n"); return 1; }
    n.SetNavigatorToType(2);
    if (a.InterruptCalls != 1 || n.PNavigator != &c) { std::printf("006cba80_TEST FAIL\n"); return 1; }
    n.SetNavigatorToType(1);
    if (c.InterruptCalls != 1 || n.PNavigator != &b) { std::printf("006cba80_TEST FAIL\n"); return 1; }
    std::printf("006cba80_TEST PASS\n");
    return 0;
}
