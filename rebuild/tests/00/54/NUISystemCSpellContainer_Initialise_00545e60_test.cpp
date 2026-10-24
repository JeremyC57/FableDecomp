#include <cstdio>
#include <vector>
static int g_baseInit = 0;
struct CChangingStateComponent {
    std::vector<CChangingStateComponent*> Children;
    void Initialise() { ++g_baseInit; }
};
struct CSpellContainer : CChangingStateComponent {
    std::vector<CChangingStateComponent*> BaseSpells; CChangingStateComponent* NewSpellComponent; unsigned long NewSpellIndex;
    void Initialise()
    {
        CChangingStateComponent::Initialise();
        for (unsigned long i = 0; i < Children.size(); ++i) BaseSpells.push_back(Children[i]);
        NewSpellComponent = 0; NewSpellIndex = 0;
    }
};
int main()
{
    CSpellContainer s; CChangingStateComponent a, b;
    s.Children.push_back(&a); s.Children.push_back(&b); s.NewSpellComponent = &a; s.NewSpellIndex = 4;
    s.Initialise();
    if (g_baseInit != 1 || s.BaseSpells.size() != 2 || s.BaseSpells[0] != &a || s.BaseSpells[1] != &b || s.NewSpellComponent != 0 || s.NewSpellIndex != 0)
        { std::printf("00545e60_TEST FAIL\n"); return 1; }
    std::printf("00545e60_TEST PASS\n");
    return 0;
}