#include <cstdio>
#include <vector>
struct CComponent { int id; };
struct CSpellContainer {
    std::vector<CComponent*> Children; std::vector<CComponent*> BaseSpells; CComponent* NewSpellComponent; unsigned long NewSpellIndex;
    void AcceptAssignments()
    {
        NewSpellComponent = 0; NewSpellIndex = 0;
        for (unsigned long i = 0; i < Children.size(); ++i) BaseSpells[i] = Children[i];
    }
};
int main()
{
    CComponent a, b, c; CSpellContainer s;
    s.Children.push_back(&a); s.Children.push_back(&b);
    s.BaseSpells.push_back(&c); s.BaseSpells.push_back(&c); s.BaseSpells.push_back(&c);
    s.NewSpellComponent = &c; s.NewSpellIndex = 5;
    s.AcceptAssignments();
    if (s.NewSpellComponent != 0 || s.NewSpellIndex != 0 || s.BaseSpells[0] != &a || s.BaseSpells[1] != &b || s.BaseSpells[2] != &c)
        { std::printf("00545670_TEST FAIL\n"); return 1; }
    std::printf("00545670_TEST PASS\n");
    return 0;
}