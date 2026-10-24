#include <cstdio>
#include <vector>
#include <map>
struct C2DVector { float X, Y; };
struct CUIState { C2DVector Position; };
struct CComponent {
    std::map<unsigned long, CUIState> States; std::vector<CComponent*> Children;
    std::map<unsigned long, CUIState>& GetStates() { return States; }
    void AddChild(CComponent* c) { Children.push_back(c); }
};
struct CSpellContainer : CComponent {
    C2DVector PositionOffset;
    void AddChildAndSetOffsets(CComponent* child)
    {
        child->GetStates()[0].Position.X = PositionOffset.X * (float)Children.size();
        AddChild(child);
    }
};
int main()
{
    CSpellContainer s; CComponent a, b; s.PositionOffset.X = 10.0f; s.PositionOffset.Y = 3.0f;
    s.AddChildAndSetOffsets(&a); s.AddChildAndSetOffsets(&b);
    if (s.Children.size() != 2 || a.States[0].Position.X != 0.0f || b.States[0].Position.X != 10.0f || s.Children[1] != &b)
        { std::printf("00545df0_TEST FAIL\n"); return 1; }
    std::printf("00545df0_TEST PASS\n");
    return 0;
}