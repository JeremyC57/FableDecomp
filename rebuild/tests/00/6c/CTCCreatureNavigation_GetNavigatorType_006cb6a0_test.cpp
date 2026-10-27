// Behaviour test for CTCCreatureNavigation::GetNavigatorType (0x006CB6A0): returns the navigator's class Type word.
#include <cstdio>
class CClass { public: unsigned int Type; unsigned int GetType() const { return Type; } };
class CNavigatorBase : public CClass { public: };
class CTCCreatureNavigation { public:
    CNavigatorBase* PNavigator;
    unsigned int GetNavigatorType() const { return PNavigator->GetType(); }
};
int main()
{
    CNavigatorBase n; n.Type = 0x1234;
    CTCCreatureNavigation c; c.PNavigator = &n;
    if (c.GetNavigatorType() != 0x1234) { std::printf("006cb6a0_TEST FAIL\n"); return 1; }
    n.Type = 7;
    if (c.GetNavigatorType() != 7) { std::printf("006cb6a0_TEST FAIL\n"); return 1; }
    std::printf("006cb6a0_TEST PASS\n");
    return 0;
}